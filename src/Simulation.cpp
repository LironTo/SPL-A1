#include "Simulation.h"

Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0), actionsLog(vector<BaseAction*>()), 
                                                        plans(vector<Plan>()), settlements(vector<Settlement*>()), 
                                                        facilitiesOptions(vector<FacilityType>()) {
    std::fstream configFile(configFilePath);
    string line;
    while(getline(configFile, line)) {
        if(line.empty()) { continue; }
        if(line.at(0) == '#') { continue; }
        vector<string> lineargs = Auxiliary::parseArguments(line);
        if(lineargs[0] == "settlement") {
            addSettlement(new Settlement(lineargs[1], SettlementType(stoi(lineargs[2]))));
        } 
        else if(lineargs[0] == "facility") {
            addFacility(FacilityType(lineargs[1], FacilityCategory(stoi(lineargs[2])), stoi(lineargs[3]), 
                                                                    stoi(lineargs[4]), stoi(lineargs[5]), stoi(lineargs[6])));
        }
        else if(lineargs[0] == "plan") {
            addPlanHelper(lineargs);
        }
    }
    configFile.close();
}

void Simulation::start() {
    open();
    while(isRunning){
        string command;
        getline(cin,command);
        vector<string> lineargs = Auxiliary::parseArguments(command);

        BaseAction* action = nullptr;
        if(lineargs.at(0) == "step") { action = new SimulateStep(stoi(lineargs.at(1))); }
        else if(lineargs.at(0) == "plan") { action = new AddPlan(lineargs.at(1), lineargs.at(2)); }
        else if(lineargs.at(0) == "settlement") { action = new AddSettlement(lineargs.at(1), SettlementType(stoi(lineargs.at(2)))); }
        else if(lineargs.at(0) == "facility") { action = new AddFacility(lineargs.at(1), FacilityCategory(stoi(lineargs.at(2))), stoi(lineargs.at(3)), stoi(lineargs.at(4)), stoi(lineargs.at(5)), stoi(lineargs.at(6))); }
        else if(lineargs.at(0) == "planStatus") { action = new PrintPlanStatus(stoi(lineargs.at(1))); }
        else if(lineargs.at(0) == "changePolicy") { action = new ChangePlanPolicy(stoi(lineargs.at(1)), lineargs.at(2)); }
        else if(lineargs.at(0) == "log") { action = new PrintActionsLog(); }
        else if(lineargs.at(0) == "close") { action = new Close(); }
        else if(lineargs.at(0) == "backup") { action = new BackupSimulation(); }
        else if(lineargs.at(0) == "restore") { action = new RestoreSimulation(); }
        else { cout << "Invalid command" << endl; }

        if(action != nullptr){
            action->act(*this);
            addAction(action);
        }
    }
}

void Simulation::addPlan(const Settlement& settlement, SelectionPolicy *selectionPolicy) {
    plans.push_back(Plan(planCounter, settlement, selectionPolicy, facilitiesOptions));
    planCounter++;
}

bool Simulation::addPlanHelper(vector<string>& lineargs) {
    if(lineargs.size() != 3) { return false; }
    if(!isSettlementExists(lineargs.at(1))) { return false; }
    Settlement &relevant_settlement = getSettlement(lineargs.at(1));
    string relevant_policy = lineargs.at(2);
    if(relevant_policy == "nve") { addPlan(relevant_settlement, new NaiveSelection()); }
    else if(relevant_policy == "bal") { addPlan(relevant_settlement, new BalancedSelection(0,0,0)); }
    else if(relevant_policy == "eco") { addPlan(relevant_settlement, new EconomySelection()); }
    else if(relevant_policy == "env") { addPlan(relevant_settlement, new SustainabilitySelection()); }
    else { return false; }
    return true;
}

void Simulation::addAction(BaseAction *action) {
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement) {
    if(isSettlementExists(settlement->getName())) { return false; }
    settlements.push_back(settlement);
    return true;
}

bool Simulation::addFacility(FacilityType facility) {
    if(isFacilityExists(facility.getName())) { return false; }
    facilitiesOptions.push_back(facility);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName) {
    for(Settlement* settlement : settlements) {
        if(settlement->getName() == settlementName) { 
            return true; 
        }
    }
    return false;
}

bool Simulation::isFacilityExists(const string &facilityName) {
    for(FacilityType facility : facilitiesOptions) {
        if(facility.getName() == facilityName) { return true; }
    }
    return false;
}

Settlement &Simulation::getSettlement(const string &settlementName) {
    long unsigned int index = 0;
    for(long unsigned int i = 0; i < settlements.size(); i++){
        if(settlements.at(i)->getName() == settlementName) { 
            index = i; 
            break;
        }
    }
    return *(settlements.at(index));
}

Settlement &Simulation::getSettlementC(const string &settlementName) const {
    long unsigned int index = 0;
    for(long unsigned int i = 0; i < settlements.size(); i++){
        if(settlements.at(i)->getName() == settlementName) { 
            index = i; 
            break;
        }
    }
    return *(settlements.at(index));
}

Plan &Simulation::getPlan(const int planID) {
    if((long unsigned int)planID >= plans.size()) throw out_of_range("Plan doesn't exist");
    return plans[planID];
}

void Simulation::step() {
    for(Plan& plan : plans) {
        plan.step();
    }
}

void Simulation::close() {
    for(Plan& plan : plans) {
        cout << plan.toString() << endl;
    }
    isRunning = false;
}

void Simulation::open() {
    isRunning = true;
    cout << "The simulation has started" << endl;
}

void Simulation::printActionsLog() {
    for(BaseAction* action : actionsLog) {
        cout << action->toString() << endl;
    }
}

Simulation::~Simulation() {
    for(Settlement* settlement : settlements) {
        delete settlement;
    }
    for(BaseAction* action : actionsLog) {
        delete action;
    }
}

Simulation::Simulation(const Simulation &other)
    : isRunning(other.isRunning), 
      planCounter(other.planCounter), 
      actionsLog(vector<BaseAction*>()), 
      plans(vector<Plan>()),
      settlements(vector<Settlement*>()), 
      facilitiesOptions(vector<FacilityType>()) { 
    for (const FacilityType& facility : other.facilitiesOptions) {
        facilitiesOptions.push_back(FacilityType(facility));
    }
    for (Settlement* settlement : other.settlements) {
        settlements.push_back(new Settlement(*settlement));
    }
    for (const Plan& plan : other.plans) { 
        plans.push_back(Plan(plan, *this)); 
    }
    for (BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }
}


Simulation &Simulation::operator=(const Simulation &other){  
    if(this != &other){
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        for(BaseAction* action: actionsLog) {delete action;}
        actionsLog.clear();
        plans.clear();
        for(Settlement* settlement: settlements) {delete settlement;}
        settlements.clear();
        facilitiesOptions.clear();
        for(const Settlement* settlement: other.settlements) {settlements.push_back(new Settlement(*settlement));}
        for(const FacilityType& facility: other.facilitiesOptions) {facilitiesOptions.push_back(FacilityType(facility));}
        for(const Plan& plan: other.plans) {plans.push_back(Plan(plan, *this));}
        for(const BaseAction* action: other.actionsLog) {actionsLog.push_back(action->clone());}
    }
    return *this;
}

const vector<FacilityType> &Simulation::getFacilityOptions() const {
    return facilitiesOptions;
}

Simulation::Simulation(Simulation &&other) 
    : isRunning(other.isRunning), 
      planCounter(other.planCounter),
        actionsLog(),
        plans(),
        settlements(),
        facilitiesOptions() {
            for(BaseAction* action: actionsLog) {delete action;}
            actionsLog.clear();
            plans.clear();
            for(Settlement* settlement: settlements) {delete settlement;}
            settlements.clear();
            facilitiesOptions.clear();
            for(const Settlement* settlement: other.settlements) {
                settlements.push_back(new Settlement(*settlement)); 
                delete settlement;
            }
            for(const FacilityType& facility: other.facilitiesOptions) {facilitiesOptions.push_back(FacilityType(facility));}
            for(const Plan& plan: other.plans) {plans.push_back(Plan(plan, *this));}
            for(const BaseAction* action: other.actionsLog) {
                actionsLog.push_back(action->clone());
                delete action;
            }
            other.actionsLog.clear();
            other.plans.clear();
            other.settlements.clear();
            other.facilitiesOptions.clear();
}

Simulation &Simulation::operator=(Simulation &&other) {
    if(this != &other){
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        for(BaseAction* action: actionsLog) {delete action;}
        actionsLog.clear();
        plans.clear();
        for(Settlement* settlement: settlements) {delete settlement;}
        settlements.clear();
        facilitiesOptions.clear();
        for(const Settlement* settlement: other.settlements) {
            settlements.push_back(new Settlement(*settlement)); 
            delete settlement;
        }
        for(const FacilityType& facility: other.facilitiesOptions) {facilitiesOptions.push_back(FacilityType(facility));}
        for(const Plan& plan: other.plans) {plans.push_back(Plan(plan, *this));}
        for(const BaseAction* action: other.actionsLog) {
            actionsLog.push_back(action->clone());
             delete action;
        }
        other.actionsLog.clear();
        other.plans.clear();
        other.settlements.clear();
        other.facilitiesOptions.clear();
    }
    return *this;
}