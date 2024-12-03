#include "Settlement.h"
//A stringstream associates a string object with a stream allowing you to read from the string as if it were a stream (like cin).
//Origin: https://www.geeksforgeeks.org/stringstream-c-applications/
#include <sstream>

Settlement::Settlement(const string& _name, SettlementType _type)
    : name(_name), type(_type) {}

const string& Settlement::getName() const { return name; }

SettlementType Settlement::getType() const { return type; }

//Return the data on all the fields of the object
const string Settlement::toString() const {
    string result = "Settlement Name: " + getName() + " type: ";
    //switch instead of if. 
    //if(type == settelmenttype::VILLAGE) result += "VIllage"
    switch (type) {
        case SettlementType::VILLAGE:
            result += "Village";
            break;
        case SettlementType::CITY:
            result += "City";
            break;
        case SettlementType::METROPOLIS:
            result += "Metropolis";
            break;
    }
    return result;
}