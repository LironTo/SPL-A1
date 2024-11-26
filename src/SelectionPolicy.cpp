#include "SelectionPolicy.h"
#include <iostream>

/*-------------------------------SelectionPolicy---------------------------------*/

SelectionPolicy::SelectionPolicy() {}

/*-------------------------------NaiveSelection---------------------------------*/

NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}

const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions) {
    lastSelectedIndex++;
    lastSelectedIndex = lastSelectedIndex % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const {
    return "Naive, last selected index: " + std::to_string(lastSelectedIndex);
}

NaiveSelection *NaiveSelection::clone() const {
    return new NaiveSelection(*this);
}

NaiveSelection::NaiveSelection(const NaiveSelection &other) : SelectionPolicy(), lastSelectedIndex(other.lastSelectedIndex) {}

/*-------------------------------BalancedSelection---------------------------------*/

BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore) : LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore) {}

const FacilityType &BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions){
    int minDistance = distance(facilitiesOptions[0]);
    for(int i = 1; i < facilitiesOptions.size(); i++){
        int currentDistance = distance(facilitiesOptions[i]);
        if(currentDistance < minDistance){
            minDistance = currentDistance;
        }
    }
    for(int i = 0; i < facilitiesOptions.size(); i++){
        if(distance(facilitiesOptions[i]) == minDistance){
            return facilitiesOptions[i];
        }
    }
}

const string BalancedSelection::toString() const {
    return "Balanced, LifeQualityScore: " + std::to_string(LifeQualityScore) + ", EconomyScore: " + std::to_string(EconomyScore) + ", EnvironmentScore: " + std::to_string(EnvironmentScore);
}

BalancedSelection *BalancedSelection::clone() const {
    return new BalancedSelection(*this);
}

BalancedSelection::BalancedSelection(const BalancedSelection &other) : SelectionPolicy(), LifeQualityScore(other.LifeQualityScore), EconomyScore(other.EconomyScore), EnvironmentScore(other.EnvironmentScore) {}

int BalancedSelection::distance(const FacilityType &facility) const {
    int max = facility.getLifeQualityScore() + LifeQualityScore;
    int min = max;
    if(facility.getEconomyScore() + EconomyScore > max){
        max = facility.getEconomyScore() + EconomyScore;
    }
    if(facility.getEconomyScore() + EconomyScore < min){
        min = facility.getEconomyScore() + EconomyScore;
    }
    if(facility.getEnvironmentScore() + EnvironmentScore > max){
        max = facility.getEnvironmentScore() + EnvironmentScore;
    }
    if(facility.getEnvironmentScore() + EnvironmentScore < min){
        min = facility.getEnvironmentScore() + EnvironmentScore;
    }
    return max - min;
}

void BalancedSelection::setScores(int LifeQualityScore, int EconomyScore, int EnvironmentScore) {
    this->LifeQualityScore = LifeQualityScore;
    this->EconomyScore = EconomyScore;
    this->EnvironmentScore = EnvironmentScore;
}

/*-------------------------------EconomySelection---------------------------------*/

EconomySelection::EconomySelection() : lastSelectedIndex(-1) {}

const FacilityType &EconomySelection::selectFacility(const vector<FacilityType> &facilitiesOptions) {
    lastSelectedIndex++;
    for(int i = 0; i < facilitiesOptions.size(); i++){
        int index = (lastSelectedIndex + i) % facilitiesOptions.size();
        if(facilitiesOptions[index].getCategory() == FacilityCategory::ECONOMY){
            lastSelectedIndex = index;
            break;
        }
    }
    return facilitiesOptions[lastSelectedIndex];
}

const string EconomySelection::toString() const {
    return "Economy, last selected index: " + std::to_string(lastSelectedIndex);
}

EconomySelection *EconomySelection::clone() const {
    return new EconomySelection(*this);
}

EconomySelection::EconomySelection(const EconomySelection &other) : SelectionPolicy(), lastSelectedIndex(other.lastSelectedIndex) {}

/*-------------------------------SustainabilitySelection---------------------------------*/

SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1) {}

const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions) {
    lastSelectedIndex++;
    for(int i = 0; i < facilitiesOptions.size(); i++){
        int index = (lastSelectedIndex + i) % facilitiesOptions.size();
        if(facilitiesOptions[index].getCategory() == FacilityCategory::ENVIRONMENT){
            lastSelectedIndex = index;
            break;
        }
    }
    return facilitiesOptions[lastSelectedIndex];
}

const string SustainabilitySelection::toString() const {
    return "Sustainability, last selected index: " + std::to_string(lastSelectedIndex);
}

SustainabilitySelection *SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this);
}

SustainabilitySelection::SustainabilitySelection(const SustainabilitySelection &other) : SelectionPolicy(), lastSelectedIndex(other.lastSelectedIndex) {}