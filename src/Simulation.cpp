#include "Simulation.h"
#include "Auxiliary.h"
#include <iostream>
#include <fstream>

Simulation::Simulation(const string &configFilePath) {
    isRunning = false;
    planCounter = 0;
    actionsLog = std::vector<BaseAction*>();
    plans = std::vector<Plan>();
    settlements = std::vector<Settlement*>();
    facilitiesOptions = std::vector<FacilityType>();
    configFile = new std::fstream(configFilePath);
}

void Simulation::start() {
    isRunning = true;
    string line;
    while(getline(*configFile, line)){ 
        std::vector<std::string> lineP = Auxiliary::parseArguments(line);

    }
    configFile->close();
    this->close();
}

void Simulation::addPlan(const Settlement *settlement, SelectionPolicy *selectionPolicy) {
    Plan plan(planCounter, *settlement, selectionPolicy, facilitiesOptions);
    plans.push_back(plan);
}

void Simulation::addAction(BaseAction *action) {
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement) {
    if(isSettlementExists(settlement->getName())){
        return false;
    }
    settlements.push_back(settlement);
    return true;
}