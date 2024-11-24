#include "Action.h"
#include <iostream>


/*-------------------------BaseAction------------------------*/

BaseAction::BaseAction() {}

ActionStatus BaseAction::getStatus() const {
    return status;
}

const string BaseAction::toString() const {
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
    return "step " + std::to_string(numOfSteps) + BaseAction::toString();
}

SimulateStep *SimulateStep::clone() const {
    return new SimulateStep(*this);
}

/*-------------------------AddPlan------------------------*/

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy) : settlementName(settlementName),
                                                                                selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation &simulation) { 
    Settlement *settlement = simulation.getSettlement(settlementName);
    if (settlement == nullptr) {
        error("Cannot create this plan");
        return;
    }
    if(selectionPolicy == "nve") { simulation.addPlan(settlement, new NaiveSelection()); }
    else if(selectionPolicy == "bal") { simulation.addPlan(settlement, new BalancedSelection(0,0,0)); }
    else if(selectionPolicy == "eco") { simulation.addPlan(settlement, new EconomySelection()); }
    else if(selectionPolicy == "env") { simulation.addPlan(settlement, new SustainabilitySelection()); }
    else { 
        error("Cannot create this plan");
        return;
    }
    complete();
}

const string AddPlan::toString() const {
    return "plan " + settlementName + " " + selectionPolicy + BaseAction::toString();
}

AddPlan *AddPlan::clone() const {
    return new AddPlan(*this);
}

/*-------------------------AddSettlement------------------------*/

AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType) : settlementName(settlementName),
                                                                                           settlementType(settlementType) {}

void AddSettlement::act(Simulation &simulation) {
    if (simulation.isSettlementExists(settlementName)) {
        error("Settlement already exists");
        return;
    }
    simulation.addSettlement(new Settlement(settlementName, settlementType));
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
    return "settlement " + settlementName + " " + std::to_string(type) + " " + BaseAction::toString();
}

/*-------------------------AddFacility------------------------*/

AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore) : facilityName(facilityName),
                                                                                                                                                                                                                   facilityCategory(facilityCategory),
                                                                                                                                                                                                                   price(price),
                                                                                                                                                                                                                   lifeQualityScore(lifeQualityScore),
                                                                                                                                                                                                                   economyScore(economyScore),
                                                                                                                                                                                                                   environmentScore(environmentScore) {}

void AddFacility::act(Simulation &simulation) {
    if (simulation.isFacilityExists(facilityName)) {
        error("Facility already exists");
        return;
    }
    simulation.addFacility(FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore));
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
    return "facility " + facilityName + " " + std::to_string(type) + " " + std::to_string(price) + " " + std::to_string(lifeQualityScore) + " " + std::to_string(economyScore) + " " + std::to_string(environmentScore) + BaseAction::toString();
}

