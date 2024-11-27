#include "../include/Settlement.h"
#include <iostream>

Settlement::Settlement(const string &name, SettlementType type): name(name), type(type){};
const string& Settlement::getName() const {
    return name; 
}
SettlementType Settlement::getType() const{
    return type;
}
const string Settlement::toString() const {
    string settlementTypeStr;
    switch (type) {
        case SettlementType::VILLAGE:
            settlementTypeStr = "VILLAGE";
            break;
        case SettlementType::CITY:
            settlementTypeStr = "CITY";
            break;
        case SettlementType::METROPOLIS:
            settlementTypeStr = "METROPOLIS";
            break;
        default:
            settlementTypeStr = "UNKNOWN";
    }
    return "Settlement Name: " + name + ", Type: " + settlementTypeStr;
}




