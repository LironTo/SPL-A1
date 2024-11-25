#include <iostream>
#include "../include/Plan.h"

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, 
const vector<FacilityType> &facilityOptions) : 
plan_id(planId), settlement(&settlement), selectionPolicy(selectionPolicy), 
facilityOptions(facilityOptions), status(PlanStatus::AVALIABLE),
 life_quality_score(0), economy_score(0), environment_score(0) 
{
    // Initialize the scores
    life_quality_score = 0;
    economy_score = 0;
    environment_score = 0;
    
}
int getlifeQualityScore() const {
    
}



