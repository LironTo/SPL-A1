#include <iostream>
#include "../include/Plan.h"
#include <sstream>

Plan::Plan(int planId, 
           const Settlement &settlement,
           SelectionPolicy* selectionPolicy, 
           const std::vector<FacilityType>& facilityOptions)
    : plan_id(planId), 
      settlement(settlement),
      selectionPolicy(selectionPolicy),
      facilityOptions(facilityOptions),
      status(PlanStatus::AVALIABLE),
      life_quality_score(0),
      economy_score(0),
      environment_score(0)
{}


//rule of 5
Plan::~Plan() {
    Clear();
}

Plan::Plan(const Plan& other): plan_id(other.plan_id), 
      settlement(other.settlement), 
      selectionPolicy(other.selectionPolicy->clone()), 
      status(PlanStatus::AVALIABLE),
      facilities(), 
      underConstruction(), 
      facilityOptions(other.facilityOptions),  
      life_quality_score(0), 
      economy_score(0), 
      environment_score(0) {
    for (Facility* facility : other.facilities) {
        facilities.push_back(new Facility(*facility));
    }
    for (Facility* facility : other.underConstruction) {
        underConstruction.push_back(new Facility(*facility));
      }
    };

void Plan::Clear() {
    for (Facility* facility : facilities) {
        delete facility;
    }
    for (Facility* facility : underConstruction) {
        delete facility;
    }
    delete selectionPolicy;

    // Do not delete settlement or assign nullptr
}

// Plan& Plan::operator=(const Plan& other) noexcept {
//     // Self-assignment check
//     if (this == &other) {
//         return *this;
//     }
//     Clear();
//     plan_id = other.plan_id;
//     status = other.status;
//     life_quality_score = other.life_quality_score;
//     economy_score = other.economy_score;
//     environment_score = other.environment_score;

//     // Deep copy facilities
//     for (Facility* facility : other.facilities) {
//         facilities.push_back(new Facility(*facility));
//     }
//     for (Facility* facility : other.underConstruction) {
//         underConstruction.push_back(new Facility(*facility));
//     }

//     // Deep copy selection policy
//     if (other.selectionPolicy) {
//         selectionPolicy = other.selectionPolicy->clone();
//     } else {
//         selectionPolicy = nullptr;
//     }

//     // Copy settlement (shallow copy or deep copy based on ownership)
//     settlement = other.settlement;

//     return *this; // Return the updated object
// }

// Plan::Plan()
//     : plan_id(0),
//       settlement(nullptr),
//       selectionPolicy(nullptr), 
//       status(PlanStatus::AVALIABLE),
//       facilities(), 
//       underConstruction(), 
//       facilityOptions( const vector<FacilityType>()), 
//       life_quality_score(0),
//       economy_score(0),
//       environment_score(0) {
// }

Plan::Plan(Plan&& other) noexcept
    : plan_id(other.plan_id),
      settlement(other.settlement), // Keep reference intact
      selectionPolicy(other.selectionPolicy->clone()), // Deep copy 
      status(other.status),
      facilities(std::move(other.facilities)),
      underConstruction(std::move(other.underConstruction)),
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score)
{
        for (Facility* facility : other.facilities) {
        facilities.push_back(new Facility(*facility));
    }
    for (Facility* facility : other.underConstruction) {
        underConstruction.push_back(new Facility(*facility));
      } 
}



// Plan& Plan::operator=(Plan&& other) noexcept {
//     if (this != &other) {
//         Clear(); // Clean up current object's resources
//         facilities = std::move(other.facilities);
//         underConstruction = std::move(other.underConstruction);
//         //facilityOptions = other.facilityOptions; 
//         plan_id = other.plan_id;
//         selectionPolicy = other.selectionPolicy;
//         settlement = other.settlement;
//         status = other.status;
//         life_quality_score = other.life_quality_score;
//         economy_score = other.economy_score;
//         environment_score = other.environment_score;
//     }
//     return *this;
// }





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
    delete selectionPolicy;
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
    int constructionLimit = getConstructionLimit(settlement.getType());

    // Stage 1: Check PlanStatus
    if (status == PlanStatus::AVALIABLE) {
        // Stage 2: Use the selection policy to choose new facilities
        while (underConstruction.size() < constructionLimit&&facilityOptions.empty()==false) {
            const FacilityType& selectedType = selectionPolicy->selectFacility(facilityOptions);
            Facility* newFacility = new Facility(selectedType, settlement.getName());
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

void Plan::decreaseConstructionTime() {
    for (auto it = underConstruction.begin(); it != underConstruction.end(); ) {
        Facility* facility = *it;
        FacilityStatus status = facility->step();
        if (status == FacilityStatus::OPERATIONAL) {
            facilities.push_back(facility);
            it = underConstruction.erase(it);
            life_quality_score += facility->getLifeQualityScore();
            economy_score += facility->getEconomyScore();
            environment_score += facility->getEnvironmentScore();
        } else {
            ++it;
        }
    }
}

const std::string Plan::toString() const {
    std::ostringstream oss;
    oss << "PlanID: " << plan_id << "\n"
         << settlement.toString() << "\n"
        << "PlanStatus: " << (status == PlanStatus::BUSY ? "BUSY" : "AVAILABLE") << "\n"
        << "SelectionPolicy: " << (selectionPolicy->toString()) << "\n"
        << "LifeQualityScore: " << life_quality_score << "\n"
        << "EconomyScore: " << economy_score << "\n"
        << "EnvironmentScore: " << environment_score << "\n"
        << "----- Facilities in Construction -----\n";
    for (const Facility* facility : underConstruction) {
        oss << facility->toString() << "\n";
    }
    oss << "----- Completed Facilities -----\n";
    for (const Facility* facility : facilities) {
        oss << facility->toString() << "\n";
    }
    return oss.str();
}

void Plan::printStatus() {
    std::cout << toString();
}
