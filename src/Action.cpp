#include "Action.h"
#include <iostream>
using namespace std;
extern Simulation* backup;

/*-------------------------BaseAction------------------------*/

BaseAction::BaseAction() {}

BaseAction::BaseAction(const BaseAction &other) : errorMsg(other.errorMsg),  status(other.status){}

ActionStatus BaseAction::getStatus() const {
    return status;
}

const string &BaseAction::statusStr() const {
    if (status == ActionStatus::COMPLETED) {
        return " COMPLETED";
    } else if (status == ActionStatus::ERROR) {
        return " ERROR";
    }
    return "";
}

void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg) {
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    cout << "Error: " << errorMsg << endl;
}

const string &BaseAction::getErrorMsg() const {
    return errorMsg;
}

/*-------------------------SimulateStep------------------------*/

SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation) {
    for (int i = 0; i < numOfSteps; i++) {
        simulation.step();
    }
    complete();
}

const string SimulateStep::toString() const {
    return "step " + std::to_string(numOfSteps) + BaseAction::statusStr();
}

SimulateStep *SimulateStep::clone() const {
    return new SimulateStep(*this);
}

SimulateStep::SimulateStep(const SimulateStep &other) : BaseAction(other) ,numOfSteps(other.numOfSteps) {}

/*SimulateStep &SimulateStep::operator=(const SimulateStep &other) {
    if (this != &other) {
        status = other.getStatus();
        errorMsg = other.getErrorMsg();
        numOfSteps = other.numOfSteps;
    }
    return *this;
}*/

/*-------------------------AddPlan------------------------*/

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy) : settlementName(settlementName),
                                                                                selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation &simulation) { 
    vector<string> argus = vector<string>();
    argus.push_back("plan");
    argus.push_back(settlementName);
    argus.push_back(selectionPolicy);
    try{
        if(!simulation.addPlanHelper(&argus)){
            error("Cannot create this plan");
            return;
        }
    }
    catch(exception &e){
        error("Cannot create this plan");
        return;
    }
    complete();
}

const string AddPlan::toString() const {
    return "plan " + settlementName + " " + selectionPolicy + BaseAction::statusStr();
}

AddPlan *AddPlan::clone() const {
    return new AddPlan(*this);
}

AddPlan::AddPlan(const AddPlan &other) : BaseAction(other) ,settlementName(other.settlementName), selectionPolicy(other.selectionPolicy) {}

/*-------------------------AddSettlement------------------------*/

AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType) : settlementName(settlementName),
                                                                                           settlementType(settlementType) {}

void AddSettlement::act(Simulation &simulation) {
    try{
        if(!simulation.addSettlement(new Settlement(settlementName, settlementType))){
            error("Settlement already exists");
            return;
        }
    }
    catch(exception &e){
        error("Settlement already exists");
        return;
    }
    complete();
}

AddSettlement *AddSettlement::clone() const {
    return new AddSettlement(*this);
}

const string AddSettlement::toString() const {
    int type;
    if(settlementType == SettlementType::VILLAGE) { type = 0; }
    else if(settlementType == SettlementType::CITY) { type = 1; }
    else { type = 2; }
    return "settlement " + settlementName + " " + std::to_string(type) + " " + BaseAction::statusStr();
}

AddSettlement::AddSettlement(const AddSettlement &other) : BaseAction(other) ,settlementName(other.settlementName), settlementType(other.settlementType) {}

/*-------------------------AddFacility------------------------*/

AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore) : facilityName(facilityName),
                                                                                                                                                                                                                   facilityCategory(facilityCategory),
                                                                                                                                                                                                                   price(price),
                                                                                                                                                                                                                   lifeQualityScore(lifeQualityScore),
                                                                                                                                                                                                                   economyScore(economyScore),
                                                                                                                                                                                                                   environmentScore(environmentScore) {}

void AddFacility::act(Simulation &simulation) {
    try{
        if(!simulation.addFacility(FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore))){
            error("Facility already exists");
            return;
        }
    }
    catch(exception &e){
        error("Facility already exists");
        return;
    }
    complete();
}

AddFacility *AddFacility::clone() const {
    return new AddFacility(*this);
}

const string AddFacility::toString() const {
    int type;
    if(facilityCategory == FacilityCategory::LIFE_QUALITY) { type = 0; }
    else if(facilityCategory == FacilityCategory::ECONOMY) { type = 1; }
    else { type = 2; }
    return "facility " + facilityName + " " + std::to_string(type) + " " + std::to_string(price) + " " + std::to_string(lifeQualityScore) + " " + std::to_string(economyScore) + " " + std::to_string(environmentScore) + BaseAction::statusStr();
}

AddFacility::AddFacility(const AddFacility &other) : BaseAction(other) ,facilityName(other.facilityName),
                                                     facilityCategory(other.facilityCategory),
                                                     price(other.price),
                                                     lifeQualityScore(other.lifeQualityScore),
                                                     economyScore(other.economyScore),
                                                     environmentScore(other.environmentScore) {}

/*-------------------------PrintPlanStatus------------------------*/

PrintPlanStatus::PrintPlanStatus(int planId) : planId(planId) {}

void PrintPlanStatus::act(Simulation &simulation) {
    try{
        Plan plan = simulation.getPlan(planId);
        plan.printStatus();
    }
    catch(exception &e){
        error("Plan doesn't exist");
        return;
    }
    complete();
}

PrintPlanStatus *PrintPlanStatus::clone() const {
    return new PrintPlanStatus(*this);
}

const string PrintPlanStatus::toString() const {
    return "planStatus " + std::to_string(planId) + BaseAction::statusStr();
}

PrintPlanStatus::PrintPlanStatus(const PrintPlanStatus &other) : BaseAction(other) ,planId(other.planId) {}

/*-------------------------ChangePlanPolicy------------------------*/

ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy) : planId(planId), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation) {
    try{
        Plan &plan = simulation.getPlan(planId);
        SelectionPolicy *selectionPolicy = nullptr;
        if(newPolicy == "nve") { selectionPolicy = new NaiveSelection(); }
        else if(newPolicy == "bal") { selectionPolicy = new BalancedSelection(plan.getlifeQualityScore(),plan.getEconomyScore(),plan.getEnvironmentScore()); }
        else if(newPolicy == "eco") { selectionPolicy = new EconomySelection(); }
        else if(newPolicy == "env") { selectionPolicy = new SustainabilitySelection(); }
        else{
            error("Cannot change selection policy");
            return;
        }
        plan.setSelectionPolicy(selectionPolicy);
    }
    catch(exception &e){
        error("Cannot change selection policy");
        return;
    }
    complete();
}

ChangePlanPolicy *ChangePlanPolicy::clone() const {
    return new ChangePlanPolicy(*this);
}

const string ChangePlanPolicy::toString() const {
    return "changePolicy " + std::to_string(planId) + " " + newPolicy + BaseAction::statusStr();
}

ChangePlanPolicy::ChangePlanPolicy(const ChangePlanPolicy &other) : BaseAction(other) ,planId(other.planId), newPolicy(other.newPolicy) {}

/*-------------------------PrintActionsLog------------------------*/

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Simulation &simulation) {
    simulation.printActionsLog();
    complete();
}

PrintActionsLog *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

const string PrintActionsLog::toString() const {
    return "log" + BaseAction::statusStr();
}

PrintActionsLog::PrintActionsLog(const PrintActionsLog &other) : BaseAction(other) {}

/*-------------------------Close------------------------*/

Close::Close() {}

void Close::act(Simulation &simulation) {
    simulation.close();
    complete();
}

Close *Close::clone() const {
    return new Close(*this);
}

const string Close::toString() const {
    return "close" + BaseAction::statusStr();
}

Close::Close(const Close &other) : BaseAction(other) {}

/*-------------------------BackupSimulation------------------------*/

BackupSimulation::BackupSimulation() {}

void BackupSimulation::act(Simulation &simulation) {
    if(backup!=nullptr){
        delete backup;
        backup = nullptr;
    }
    backup = new Simulation(simulation);
    complete();
}

BackupSimulation *BackupSimulation::clone() const {
    return new BackupSimulation(*this);
}

const string BackupSimulation::toString() const {
    return "backup" + BaseAction::statusStr();
}

BackupSimulation::BackupSimulation(const BackupSimulation &other) : BaseAction(other) {}

/*-------------------------RestoreSimulation------------------------*/

RestoreSimulation::RestoreSimulation() {}

void RestoreSimulation::act(Simulation &simulation) {
    if(backup==nullptr){
        error("No backup available");
        return;
    }
    simulation = *backup;
    complete();
}

RestoreSimulation *RestoreSimulation::clone() const {
    return new RestoreSimulation(*this);
}

const string RestoreSimulation::toString() const {
    return "restore" + BaseAction::statusStr();
}

RestoreSimulation::RestoreSimulation(const RestoreSimulation &other) : BaseAction(other) {}