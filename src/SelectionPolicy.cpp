#include "SelectionPolicy.h"
#include <iostream>

/*-------------------------------SelectionPolicy---------------------------------*/

SelectionPolicy::SelectionPolicy() {}

SelectionPolicy::SelectionPolicy(const SelectionPolicy &other) {}

/*-------------------------------NaiveSelection---------------------------------*/

NaiveSelection::NaiveSelection() : SelectionPolicy(), lastSelectedIndex(-1) {}

const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions) {
    lastSelectedIndex++;
    lastSelectedIndex = lastSelectedIndex % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const {
    return "nve ";
}

NaiveSelection *NaiveSelection::clone() const {
    return new NaiveSelection(*this);
}

NaiveSelection::NaiveSelection(const NaiveSelection &other) : SelectionPolicy(other), lastSelectedIndex(other.lastSelectedIndex) {}

NaiveSelection &NaiveSelection::operator=(const NaiveSelection &other) {
    if(this != &other){
        lastSelectedIndex = other.lastSelectedIndex;
    }
    return *this;
}

NaiveSelection::NaiveSelection(NaiveSelection &&other) : SelectionPolicy(other), lastSelectedIndex(other.lastSelectedIndex) {}

NaiveSelection &NaiveSelection::operator=(NaiveSelection &&other) {
    if(this != &other){
        lastSelectedIndex = other.lastSelectedIndex;
    }
    return *this;
}

/*-------------------------------BalancedSelection---------------------------------*/

BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore) : SelectionPolicy(), LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore) {}

const FacilityType &BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions){
    int minDistance = distance(facilitiesOptions.at(0));
    const FacilityType* selectedFacility = &(facilitiesOptions.at(0));
    for(long unsigned int i = 0; i < facilitiesOptions.size(); i++){
        int currentDistance = distance(facilitiesOptions.at(i));
        if(currentDistance < minDistance){
            minDistance = currentDistance;
            selectedFacility = &(facilitiesOptions.at(i));
        }
    }
    LifeQualityScore += selectedFacility->getLifeQualityScore();
    EconomyScore += selectedFacility->getEconomyScore();
    EnvironmentScore += selectedFacility->getEnvironmentScore();
    return *selectedFacility;
}

const string BalancedSelection::toString() const {
    return "bal";
}

BalancedSelection *BalancedSelection::clone() const {
    return new BalancedSelection(*this);
}

BalancedSelection::BalancedSelection(const BalancedSelection &other) : SelectionPolicy(other), LifeQualityScore(other.LifeQualityScore), EconomyScore(other.EconomyScore), EnvironmentScore(other.EnvironmentScore) {}

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

BalancedSelection &BalancedSelection::operator=(const BalancedSelection &other) {
    if(this != &other){
        LifeQualityScore = other.LifeQualityScore;
        EconomyScore = other.EconomyScore;
        EnvironmentScore = other.EnvironmentScore;
    }
    return *this;
}

BalancedSelection::BalancedSelection(BalancedSelection &&other) : SelectionPolicy(other), LifeQualityScore(other.LifeQualityScore), EconomyScore(other.EconomyScore), EnvironmentScore(other.EnvironmentScore) {}

BalancedSelection &BalancedSelection::operator=(BalancedSelection &&other) {
    if(this != &other){
        LifeQualityScore = other.LifeQualityScore;
        EconomyScore = other.EconomyScore;
        EnvironmentScore = other.EnvironmentScore;
    }
    return *this;
}

/*-------------------------------EconomySelection---------------------------------*/

EconomySelection::EconomySelection() : SelectionPolicy(), lastSelectedIndex(-1) {}

const FacilityType &EconomySelection::selectFacility(const vector<FacilityType> &facilitiesOptions) {
    lastSelectedIndex++;
    int size = facilitiesOptions.size();
    for(int i = 0; i < size; i++){
        int index = (lastSelectedIndex + i) % facilitiesOptions.size();
        if(facilitiesOptions[index].getCategory() == FacilityCategory::ECONOMY){
            lastSelectedIndex = index;
            break;
        }
    }
    return facilitiesOptions[lastSelectedIndex];
}

const string EconomySelection::toString() const {
    return "eco";
}
EconomySelection *EconomySelection::clone() const {
    return new EconomySelection(*this);
}

EconomySelection::EconomySelection(const EconomySelection &other) : SelectionPolicy(other), lastSelectedIndex(other.lastSelectedIndex) {}

EconomySelection &EconomySelection::operator=(const EconomySelection &other) {
    if(this != &other){
        lastSelectedIndex = other.lastSelectedIndex;
    }
    return *this;
}

EconomySelection::EconomySelection(EconomySelection &&other) : SelectionPolicy(other), lastSelectedIndex(other.lastSelectedIndex) {}

EconomySelection &EconomySelection::operator=(EconomySelection &&other) {
    if(this != &other){
        lastSelectedIndex = other.lastSelectedIndex;
    }
    return *this;
}

/*-------------------------------SustainabilitySelection---------------------------------*/

SustainabilitySelection::SustainabilitySelection() : SelectionPolicy(), lastSelectedIndex(-1) {}

const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions) {
    lastSelectedIndex++;
    int size = facilitiesOptions.size();
    for(int i = 0; i < size; i++){
        int index = (lastSelectedIndex + i) % facilitiesOptions.size();
        if(facilitiesOptions[index].getCategory() == FacilityCategory::ENVIRONMENT){
            lastSelectedIndex = index;
            break;
        }
    }
    return facilitiesOptions[lastSelectedIndex];
}

const string SustainabilitySelection::toString() const {
    return "env";
}

SustainabilitySelection *SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this);
}

SustainabilitySelection::SustainabilitySelection(const SustainabilitySelection &other) : SelectionPolicy(other), lastSelectedIndex(other.lastSelectedIndex) {}

SustainabilitySelection &SustainabilitySelection::operator=(const SustainabilitySelection &other) {
    if(this != &other){
        lastSelectedIndex = other.lastSelectedIndex;
    }
    return *this;
}

SustainabilitySelection::SustainabilitySelection(SustainabilitySelection &&other) : SelectionPolicy(other), lastSelectedIndex(other.lastSelectedIndex) {}

SustainabilitySelection &SustainabilitySelection::operator=(SustainabilitySelection &&other) {
    if(this != &other){
        lastSelectedIndex = other.lastSelectedIndex;
    }
    return *this;
}