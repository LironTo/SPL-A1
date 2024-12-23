#pragma once
#include <string>
#include <vector>
#include "Auxiliary.h"
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include <iostream>
#include <fstream>
#include "Action.h"

using namespace std;
using std::string;
using std::vector;

class BaseAction;


class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        Simulation(const Simulation &other);
        Simulation &operator=(const Simulation &other);
        Simulation(Simulation &&other);
        Simulation &operator=(Simulation &&other);
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        bool addPlanHelper(vector<string>& lineargs);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        bool isFacilityExists(const string &facilityName);
        Settlement &getSettlement(const string &settlementName);
        Settlement &getSettlementC(const string &settlementName) const;
        const vector<FacilityType> &getFacilityOptions() const;
        Plan &getPlan(const int planID);
        void step();
        void close();
        void open();
        void printActionsLog();
        ~Simulation();

    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;
};