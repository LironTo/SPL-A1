#include "Simulation.h"
#include "Auxiliary.h"
#include <iostream>
#include <fstream>
#include "Action.h"
using namespace std;

Simulation::Simulation(const string &configFilePath) {
    isRunning = false;
    planCounter = 0;
    actionsLog = std::vector<BaseAction*>();
    plans = std::vector<Plan>();
    settlements = std::vector<Settlement*>();
    facilitiesOptions = std::vector<FacilityType>();
    std::fstream configFile(configFilePath);
    string line;
    while(getline(configFile, line)) {
        vector<string> lineargs = Auxiliary::parseArguments(line);
        if(lineargs[0] == "settlement") {
            addSettlement(new Settlement(lineargs[1], SettlementType(stoi(lineargs[2]))));
        } 
        else if(lineargs[0] == "facility") {
            addFacility(FacilityType(lineargs[1], FacilityCategory(stoi(lineargs[2])), stoi(lineargs[3]), stoi(lineargs[4]), stoi(lineargs[5]), stoi(lineargs[6])));
        }
        else if(lineargs[0] == "plan") {
            addPlanHelper(&lineargs);
        }
    }
    configFile.close();
}

void Simulation::start() {
    open();
    cout << "The simulation has started" << endl;
    while(isRunning){
        string command;
        getline(cin,command);
        vector<string> lineargs = Auxiliary::parseArguments(command);
        for (const auto& arg : lineargs) {
            std::cout << arg << std::endl;
}
        BaseAction* action = nullptr;
        if(lineargs[0] == "step") { action = new SimulateStep(stoi(lineargs[1])); }
        else if(lineargs[0] == "plan") { action = new AddPlan(lineargs[1], lineargs[2]); }
        else if(lineargs[0] == "settlement") { action = new AddSettlement(lineargs[1], SettlementType(stoi(lineargs[2]))); }
        else if(lineargs[0] == "facility") { action = new AddFacility(lineargs[1], FacilityCategory(stoi(lineargs[2])), stoi(lineargs[3]), stoi(lineargs[4]), stoi(lineargs[5]), stoi(lineargs[6])); }
        else if(lineargs[0] == "planStatus") { action = new PrintPlanStatus(stoi(lineargs[1])); }
        else if(lineargs[0] == "changePolicy") { action = new ChangePlanPolicy(stoi(lineargs[1]), lineargs[2]); }
        else if(lineargs[0] == "log") { action = new PrintActionsLog(); }
        else if(lineargs[0] == "close") { action = new Close(); }
        else if(lineargs[0] == "backup") { action = new BackupSimulation(); }
        else if(lineargs[0] == "restore") { action = new RestoreSimulation(); }
        else { cout << "Invalid command" << endl; }

        if(action != nullptr){
            action->act(*this);
            addAction(action);
        }
    }
}

void Simulation::addPlan(const Settlement *settlement, SelectionPolicy *selectionPolicy) {
    plans.push_back(Plan(planCounter, *settlement, selectionPolicy, facilitiesOptions));
    planCounter++;
}

bool Simulation::addPlanHelper(vector<string>* lineargs) {
    if(lineargs->size() != 3) { return false; }
    if(!isSettlementExists(lineargs->at(1))) { return false; }
    if(lineargs->at(2) == "nve") { addPlan(getSettlement(lineargs->at(1)), new NaiveSelection()); }
    else if(lineargs->at(2) == "bal") { addPlan(getSettlement(lineargs->at(1)), new BalancedSelection(0,0,0)); }
    else if(lineargs->at(2) == "eco") { addPlan(getSettlement(lineargs->at(1)), new EconomySelection()); }
    else if(lineargs->at(2) == "env") { addPlan(getSettlement(lineargs->at(1)), new SustainabilitySelection()); }
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
        if(settlement->getName() == settlementName) { return true; }
    }
    return false;
}

bool Simulation::isFacilityExists(const string &facilityName) {
    for(FacilityType facility : facilitiesOptions) {
        if(facility.getName() == facilityName) { return true; }
    }
    return false;
}

Settlement *Simulation::getSettlement(const string &settlementName) {
    for(Settlement* settlement : settlements) {
        if(settlement->getName() == settlementName) { return settlement; }
    }
    return nullptr;
}

Plan &Simulation::getPlan(const int planID) {
    if(planID >= plans.size()) throw out_of_range("Plan doesn't exist");
    return plans[planID];
}

void Simulation::step() {
    for(Plan plan : plans) {
        plan.step();
    }
}

void Simulation::close() {
    for(Plan plan : plans) {
        cout << plan.toString() << endl;
    }
    isRunning = false;
    this->~Simulation();
}

void Simulation::open() {
    isRunning = true;
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

Simulation::Simulation(const Simulation &other) : isRunning(other.isRunning), planCounter(other.planCounter) {
    actionsLog = std::vector<BaseAction*>();
    for(BaseAction* action: other.actionsLog) {actionsLog.push_back(action->clone());}
    plans = std::vector<Plan>();
    for(Plan plan: other.plans) {plans.push_back(Plan(plan));}
    settlements = std::vector<Settlement*>();
    for(Settlement* settlement: other.settlements) {settlements.push_back(new Settlement(*settlement));}
    facilitiesOptions = std::vector<FacilityType>();
    for(FacilityType facility: other.facilitiesOptions) {facilitiesOptions.push_back(FacilityType(facility));}
}


Simulation &Simulation::operator=(const Simulation &other){  
    if(this != &other){
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        actionsLog = other.actionsLog;
     //   plans = other.plans;
        settlements = other.settlements;
       // facilitiesOptions = other.facilitiesOptions;
    }
    return *this;
}