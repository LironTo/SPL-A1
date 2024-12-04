#include <iostream>
#include "../include/Facility.h"
#include <string>

/*------------------------------FacilityType----------------------------*/

FacilityType::FacilityType(const string &name, 
const FacilityCategory category, 
const int price, 
const int lifeQuality_score, 
const int economy_score, 
const int environment_score): name(name), category(category), price(price),
lifeQuality_score(lifeQuality_score), economy_score(economy_score),
 environment_score(environment_score){}

int FacilityType::getCost() const {
    return price;
}
int FacilityType::getLifeQualityScore() const {
    return lifeQuality_score;
}
int FacilityType::getEnvironmentScore() const {
    return economy_score;
}
int FacilityType::getEconomyScore() const {
    return environment_score;
}
FacilityCategory FacilityType::getCategory() const {
    return category;
}
const string& FacilityType::getName() const {
    return name;
}

const string FacilityType::toString() const {
    string category_str = "NONE";
    if (category == FacilityCategory::LIFE_QUALITY) {
        category_str = "LIFE_QUALITY";
    } else if (category == FacilityCategory::ECONOMY) {
        category_str = "ECONOMY";
    } else {
        category_str = "ENVIRONMENT";
    }
    string s = "FacilityName: " + name;
    s += ", Category: " + category_str;
    s += ", Price: " + std::to_string(price);
    s += ", LifeQualityScore: " + std::to_string(lifeQuality_score);
    s += ", EconomyScore: " + std::to_string(economy_score);
    s += ", EnvironmentScore: " + std::to_string(environment_score);
    return s;
}

/*------------------------------Facility----------------------------*/

Facility::Facility(const FacilityType &type, const string &settlementName)
    : FacilityType(type), 
      settlementName(settlementName), 
      status(FacilityStatus::UNDER_CONSTRUCTIONS), 
      timeLeft(0) {} // Initialize timeLeft to 0, modify if required during construction

Facility::Facility(const string &name,
                   const string &settlementName, 
                   const FacilityCategory category, 
                   const int price, 
                   const int lifeQuality_score, 
                   const int economy_score, 
                   const int environment_score)
    : FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score), 
      settlementName(settlementName), 
      status(FacilityStatus::UNDER_CONSTRUCTIONS), 
      timeLeft(0) {} // Initialize timeLeft to 0, modify if required during construction
const string& Facility::getSettlementName() const {
    return settlementName;
}

const FacilityStatus& Facility::getStatus() const {
    return status;
}

const int Facility::getTimeLeft() const {
    return timeLeft;
}

FacilityStatus Facility::step(){
    if(timeLeft==1){
        timeLeft--;
        status=FacilityStatus::OPERATIONAL;
    }
    else {timeLeft--;}
    return status;
}

void Facility::setStatus(FacilityStatus status_) {
    status = status_;
}
const string Facility::toString() const {
    string s = "FacilityName: " + name; // Assuming `name` is a member of FacilityType
   // s += ", Status: " +(status == FacilityStatus::UNDER_CONSTRUCTIONS ? "UNDER_CONSTRUCTIONS" : "OPERATIONAL");
    s += ", Time Left: " + std::to_string(timeLeft);
    return s;
}





