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
 void Plan::step(){
    if(status==PlanStatus::BUSY){
        
    }
 }




 




