#include "Settlement.h"
//A stringstream associates a string object with a stream allowing you to read from the string as if it were a stream (like cin).
//Origin: https://www.geeksforgeeks.org/stringstream-c-applications/
#include <sstream>

Settlement::Settlement(const string &name, SettlementType)
    : name(name), type(type) {}

const string &Settlement::getName() const{ return name; }

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

int Settlement::getConstructionLimit() const {
    switch (type) {
        case SettlementType::VILLAGE: return 1; // Villages can build 1 facility at a time
        case SettlementType::CITY: return 2;    // Cities can build 2 facilities at a time
        case SettlementType::METROPOLIS: return 3; // Metropolises can build 3 facilities at a time
        default: return 0; // Default case to catch Errors
    }
}

