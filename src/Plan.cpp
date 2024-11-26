#include <iostream>
#include "../include/Plan.h"

Plan::Plan(const int planId, 
           const Settlement &settlement, 
           SelectionPolicy *selectionPolicy, 
           const vector<FacilityType> &facilityOptions) 
    : plan_id(planId), 
      settlement(&settlement), 
      selectionPolicy(selectionPolicy), 
      facilityOptions(facilityOptions), 
      status(PlanStatus::AVALIABLE), 
      facilities(), 
      underConstruction(), 
      life_quality_score(0), 
      economy_score(0), 
      environment_score(0) 
{
    // Additional validation or logic can go here, if needed
    if (!selectionPolicy) {
        throw std::invalid_argument("SelectionPolicy cannot be null");
    }
}

const int Plan::getlifeQualityScore() const {
    return life_quality_score;
}

const int Plan::getEconomyScore() const {
    return economy_score;
}
const int Plan::getEnvironmentScore() const {
    return environment_score;
}

void Plan::setSelectionPolicy(SelectionPolicy *_selectionPolicy) {
    selectionPolicy= _selectionPolicy;
}
int getConstructionLimit(SettlementType type) {
    switch (type) {
        case SettlementType::VILLAGE: return 1;
        case SettlementType::CITY: return 2;
        case SettlementType::METROPOLIS: return 3;
        default: throw std::runtime_error("Invalid SettlementType");
    }
}
void Plan::step() {
    // Calculate the construction limit once
    int constructionLimit = getConstructionLimit(settlement->getType());
    // Stage 1: Check PlanSta
    if (status == PlanStatus::AVALIABLE) {
        // Stage 2: Use the selection policy to choose new facilities
        while (underConstruction.size() < constructionLimit) {
            if (facilityOptions.empty()) {
                std::cerr << "No facility options available for selection." << std::endl;
                break; // Prevent infinite loop if no options exist
            }
            const FacilityType& selectedType = selectionPolicy->selectFacility(facilityOptions);
            Facility* newFacility = new Facility(selectedType, settlement->getName());
            underConstruction.push_back(newFacility);
        }
    }
    // Stage 3: Progress construction
    decreaseConstructionTime();
    // Stage 4: Update PlanStatus
    if (underConstruction.size() == constructionLimit) {
        status = PlanStatus::BUSY; // All slots are still occupied
    } else {
        status = PlanStatus::AVALIABLE; // Some slots are now free
    }
}

void Plan::printStatus() {
    // Print Plan ID
    std::cout << "PlanID: " << plan_id << std::endl;

    // Print Settlement Name
    if (settlement) {
        std::cout << "SettlementName: " << settlement->getName() << std::endl;
    } else {
        std::cout << "SettlementName: N/A" << std::endl;
    }

    // Print Plan Status
    std::cout << "PlanStatus: " 
              << (status == PlanStatus::BUSY ? "BUSY" : "AVAILABLE") 
              << std::endl;

    // Print Selection Policy
    if (selectionPolicy) {
        std::cout << "SelectionPolicy: " << selectionPolicy->toString() << std::endl;
    } else {
        std::cout << "SelectionPolicy: N/A" << std::endl;
    }

    // Print Scores
    std::cout << "LifeQualityScore: " << life_quality_score << std::endl;
    std::cout << "EconomyScore: " << economy_score << std::endl;
    std::cout << "EnvironmentScore: " << environment_score << std::endl;

    // Print Facilities in the Plan
    std::cout << "----- Facilities in Construction -----" << std::endl;
    for (const Facility* facility : underConstruction) {
        std::cout << facility->toString() << std::endl;
    }

    std::cout << "----- Completed Facilities -----" << std::endl;
    for (const Facility* facility : facilities) {
        std::cout << facility->toString() << std::endl;
    }
}

void Plan::decreaseConstructionTime() {
    for (auto it = underConstruction.begin(); it != underConstruction.end(); ) {
        Facility* facility = *it;
        // Call Facility::step() to progress constructionnn
        FacilityStatus status = facility->step();
        // If the facility is now operational, move it to the facilities list
        if (status == FacilityStatus::OPERATIONAL) {
            facilities.push_back(facility);     // Add to operational facilities
            it = underConstruction.erase(it);  // Remove from underConstruction
        } else {
            ++it; // Move to the next facility
        }
    }
}

const std::vector<Facility*>& Plan::getFacilities() const {
    int i=1;
    return facilities; // Return a reference to the vector
}



