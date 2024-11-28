#include <iostream>
#include "../include/Plan.h"
#include <sstream>

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

//rule of 5
Plan::~Plan() {
    Clear();
}

Plan::Plan(const Plan& other): plan_id(other.plan_id), 
      settlement(other.settlement), 
      selectionPolicy(other.selectionPolicy), 
      facilityOptions(other.facilityOptions), 
      status(PlanStatus::AVALIABLE), 
      facilities(), 
      underConstruction(), 
      life_quality_score(0), 
      economy_score(0), 
      environment_score(0) {};

void Plan::Clear(){
    for (Facility* facility : facilities) {
        delete facility;
    }
    facilities.clear();
    for (Facility* facility : underConstruction) {
        delete facility;
    }
    underConstruction.clear();
    if (selectionPolicy) {
        delete selectionPolicy;
    }
    if (settlement) {
        delete settlement;
    }
}
Plan& Plan::operator=(const Plan& other) noexcept {
    // Self-assignment check
    if (this == &other) {
        return *this;
    }
    Clear();
    plan_id = other.plan_id;
    status = other.status;
    life_quality_score = other.life_quality_score;
    economy_score = other.economy_score;
    environment_score = other.environment_score;

    // Deep copy facilities
    for (Facility* facility : other.facilities) {
        facilities.push_back(new Facility(*facility));
    }
    for (Facility* facility : other.underConstruction) {
        underConstruction.push_back(new Facility(*facility));
    }

    // Deep copy selection policy
    if (other.selectionPolicy) {
        selectionPolicy = other.selectionPolicy->clone();
    } else {
        selectionPolicy = nullptr;
    }

    // Copy settlement (shallow copy or deep copy based on ownership)
    settlement = other.settlement;

    return *this; // Return the updated object
}

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
    : facilities(std::move(other.facilities)),               
      underConstruction(std::move(other.underConstruction)),   
      facilityOptions(other.facilityOptions),                  
      plan_id(other.plan_id),                                 
      settlement(other.settlement),                            
      selectionPolicy(other.selectionPolicy),                 
      status(other.status),                                    
      life_quality_score(other.life_quality_score),            
      economy_score(other.economy_score),                    
      environment_score(other.environment_score)          
{
    other.selectionPolicy = nullptr;
    other.settlement = nullptr;
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

const std::string Plan::toString() const {
    std::ostringstream oss;
    oss << "PlanID: " << plan_id << "\n"
        << "Settlement Name: " << (settlement ? settlement->toString() : "N/A") << "\n"
        << "PlanStatus: " << (status == PlanStatus::BUSY ? "BUSY" : "AVAILABLE") << "\n"
        << "SelectionPolicy: " << (selectionPolicy ? selectionPolicy->toString() : "N/A") << "\n"
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

