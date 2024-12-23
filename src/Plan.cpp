#include <iostream>
#include "../include/Plan.h"
#include <sstream>
#include "Simulation.h"

Plan::Plan(int planId, 
           const Settlement &settlement,
           SelectionPolicy* selectionPolicy, 
           const std::vector<FacilityType>& facilityOptions)
    : plan_id(planId),
     settlement(settlement),
     selectionPolicy(selectionPolicy),
      status(PlanStatus::AVALIABLE),
      facilities(),
      underConstruction(),
      facilityOptions(facilityOptions),
      life_quality_score(0),
      economy_score(0),
      environment_score(0)
{}



//rule of 5
Plan::~Plan() {
    Clear();
}

Plan::Plan(const Plan& other,const Simulation& simulation)
    : plan_id(other.plan_id),
      settlement(simulation.getSettlementC(other.settlement.getName())),
      selectionPolicy(other.selectionPolicy->clone()),
      status(other.status),
      facilities(),
      underConstruction(),
      facilityOptions(simulation.getFacilityOptions()),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score) {
    for (Facility* facility : other.facilities) {
        facilities.push_back(new Facility(*facility));
    }
    for (Facility* facility : other.underConstruction) {
        underConstruction.push_back(new Facility(*facility));
    }
}

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

Plan::Plan(Plan&& other)
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
    string before = selectionPolicy->toString();
    if(before == "nve") { before = "Naive";}
    else if(before == "bal"){ before = "Balanced";}
    else if(before == "env"){ before = "Sustainability";}
    else if(before == "eco"){ before = "Economy";}
    

    string after = _selectionPolicy->toString();
    if(after == "nve") { after = "Naive";}
    else if(after == "bal"){ after = "Balanced";}
    else if(after == "env"){ after = "Sustainability";}
    else if(after == "eco"){ after = "Economy";}
    
    if(before == after){ 
        delete _selectionPolicy;
        throw std::runtime_error("Same selection policy"); 
    }

    std::cout << "planID: " << plan_id << std::endl;
    std::cout << "previousPolicy: " << before << std::endl;
    std::cout << "newPolicy: " << after << std::endl;

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
    long unsigned int constructionLimit = getConstructionLimit(settlement.getType());

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
    std::string result = "";
    result += "PlanID: " + std::to_string(plan_id) + "\n";
    result += "SettlementName: " + settlement.getName() + "\n";
    result += "LifeQualityScore: " + std::to_string(life_quality_score) + "\n";
    result += "EconomyScore: " + std::to_string(economy_score) + "\n";
    result += "EnvironmentScore: " + std::to_string(environment_score);
    return result;
}

void Plan::printStatus() {
    std::string result = "";
    result += "PlanID: " + std::to_string(plan_id) + "\n";
    result += "SettlementName: " + settlement.getName() + "\n";
    
    if (status == PlanStatus::BUSY) {
        result += "PlanStatus: BUSY\n";
    } else {
        result += "PlanStatus: AVAILABLE\n";
    }

    // Check if selectionPolicy is not null before dereferencing
    if (selectionPolicy) {
        result += "SelectionPolicy: " + selectionPolicy->toString() + "\n";
    } else {
        result += "SelectionPolicy: null\n";
    }

    result += "LifeQualityScore: " + std::to_string(life_quality_score) + "\n";
    result += "EconomyScore: " + std::to_string(economy_score) + "\n";
    result += "EnvironmentScore: " + std::to_string(environment_score) + "\n";

    // Iterate over facilities
    if (!facilities.empty()) {
        for (const Facility* facility : facilities) {
            if (facility) {
                result += "FacilityName: " + facility->getName() + "\n";
                if (facility->getStatus() == FacilityStatus::UNDER_CONSTRUCTIONS) {
                    result += "FacilityStatus: UNDER_CONSTRUCTION\n";
                } else {
                    result += "FacilityStatus: OPERATIONAL\n";
                }
            } else {
                result += "null facility\n";
            }
        }
    }

    // Iterate over underConstruction
    if (!underConstruction.empty()) {
        for (const Facility* facility : underConstruction) {
            if (facility) {
                result += "FacilityName: " + facility->getName() + "\n";
                if (facility->getStatus() == FacilityStatus::UNDER_CONSTRUCTIONS) {
                    result += "FacilityStatus: UNDER_CONSTRUCTION\n";
                } else {
                    result += "FacilityStatus: OPERATIONAL\n";
                }
            } else {
                result += "null facility\n";
            }
        }
    }
    std::cout << result;
}
