#include <iostream>
#include "../include/Facility.h"
#include <string>

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
    s += ", Status: " + (status == FacilityStatus::UNDER_CONSTRUCTIONS ? "UNDER_CONSTRUCTIONS" : "OPERATIONAL");
    s += ", Time Left: " + std::to_string(timeLeft);
    return s;
}





