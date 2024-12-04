#include "SelectionPolicy.h"
#include <climits>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    lastSelectedIndex = (++lastSelectedIndex) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
    //add empty facilitiesOptions verctor case
}

const string NaiveSelection::toString() const {
    return "nve";
}

NaiveSelection* NaiveSelection::clone() const {
    return new NaiveSelection(*this);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BalancedSelection::BalancedSelection(int _LifeQualityScore, int _EconomyScore, int _EnvironmentScore) : LifeQualityScore(_LifeQualityScore), EconomyScore(_EconomyScore), EnvironmentScore(_EnvironmentScore) {}

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    vector<FacilityType>::const_iterator itr;
    const FacilityType* selected;
    int distance = INT_MAX;
    for (itr = facilitiesOptions.begin(); itr != facilitiesOptions.end(); itr++) {
        int life = LifeQualityScore + (itr -> getLifeQualityScore());
        int eco = EconomyScore + (itr -> getEconomyScore());
        int env = EnvironmentScore + (itr -> getEnvironmentScore());
        int result = std::max(life, std::max(eco, env));
        if (result < distance) {
            selected = &(*itr);
            distance = result;
        }
    }
    return std::move(*selected);
}

const string BalancedSelection::toString() const {
    return "bal";
}

BalancedSelection* BalancedSelection::clone() const {
    return new BalancedSelection(*this);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


EconomySelection::EconomySelection() : lastSelectedIndex(-1) {}

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    lastSelectedIndex = (++lastSelectedIndex) % facilitiesOptions.size();
    vector<FacilityType>::const_iterator start = facilitiesOptions.begin() + lastSelectedIndex;
    // starts the iterator after the last time an economy type facility was chosen to and looks for the next one
    for (vector<FacilityType>::const_iterator f_itr = start; f_itr != facilitiesOptions.end(); f_itr++) {
        if (f_itr -> getCategory() == FacilityCategory::ECONOMY) {
            lastSelectedIndex = std::distance(start, f_itr);
            return std::move(*f_itr);
        }
    }
    // if there were no more economy type facilities left until the end of the vector it wraps back to the beginning of the vector to look for the next one
    for (start = facilitiesOptions.begin(); start != facilitiesOptions.end(); start++) {
        if (start -> getCategory() == FacilityCategory::ECONOMY) {
            lastSelectedIndex = std::distance(facilitiesOptions.begin(), start);
            return std::move(*start);
        }
    }
}

const string EconomySelection::toString() const {
    return "eco";
}

EconomySelection* EconomySelection::clone() const {
    return new EconomySelection(*this);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1) {}

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    lastSelectedIndex = (++lastSelectedIndex) % facilitiesOptions.size();
    vector<FacilityType>::const_iterator start = facilitiesOptions.begin() + lastSelectedIndex;
    // starts the iterator after the last time an enviroment type facility was chosen to and looks for the next one
    for (vector<FacilityType>::const_iterator f_itr = start; f_itr != facilitiesOptions.end(); f_itr++) {
        if (f_itr -> getCategory() == FacilityCategory::ENVIRONMENT) {
            lastSelectedIndex = std::distance(start, f_itr);
            return std::move(*f_itr);
        }
    }
    // if there were no more enviroment type facilities left until the end of the vector it wraps back to the beginning of the vector to look for the next one
    for (start = facilitiesOptions.begin(); start != facilitiesOptions.end(); start++) {
        if (start -> getCategory() == FacilityCategory::ENVIRONMENT) {
            lastSelectedIndex = std::distance(facilitiesOptions.begin(), start);
            return std::move(*start);
        }
    }
}

const string SustainabilitySelection::toString() const {
    return "env";
}

SustainabilitySelection* SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this);
}