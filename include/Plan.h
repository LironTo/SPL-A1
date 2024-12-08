#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"

using std::vector;

class Simulation;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        Plan(const Plan& other,const Simulation& simulation);    
        Plan(Plan&& other);
        void Clear();
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        ~Plan();
        void printStatus();
        void decreaseConstructionTime();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;
        Plan &operator=(const Plan &other) = delete;
        Plan &operator=(Plan &&other) = delete;
        Plan(const Plan &other) = delete;

private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy;
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};