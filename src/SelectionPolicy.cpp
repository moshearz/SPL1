#include "SelectionPolicy.h"
#include <iostream>
#include <cmath>
using namespace std;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) override {
    lastSelectedIndex = (lastSelectedIndex++) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
    //add empty facilitiesOptions verctor case
}

const string NaiveSelection::toString() override {
    return "nve";
}

NaiveSelection* NaiveSelection::clone() const  override {
    return new NaiveSelection(*this);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BalancedSelection::BalancedSelection(int _LifeQualityScore, int _EconomyScore, int _EnvironmentScore) : LifeQualityScore(_LifeQualityScore), EconomyScore(_EconomyScore), EnvironmentScore(_EnvironmentScore) {}

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) override {
    vector<FacilityType>::iterator itr
    FacilityType* selected;
    int distance = INT_MAX;
    for (itr = facilitiesOptions.begin(); itr != facilitiesOptions.end(); itr++) {
        int life = LifeQualityScore + (iter -> getLifeQualityScore());
        int eco = EconomyScore + (iter -> getEconomyScore());
        int env = EnvironmentScore + (iter -> getEnvironmentScore());
        int result = std::max(life, std::max(eco, env));
        if (result < distance) {
            selected = itr;
            distance = result
        }
    }
    return *selected;
}

const string BalancedSelection::toString() override {
    return "bal";
}

BalancedSelection* BalancedSelection::clone() const override {
    return new BalancedSelection(*this);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


EconomySelection::EconomySelection() : lastSelectedIndex(-1) {}

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) override {
    lastSelectedIndex = (lastSelectedIndex++) % facilitiesOptions.size();
    vector<FacilityType>::iterator start = facilitiesOptions.begin() + lastSelectedIndex;
    // starts the iterator after the last time an economy type facility was chosen to and looks for the next one
    for (vector<FacilityType>::iterator f_itr = start; f_itr != facilitiesOptions.end(); f_itr++) {
        if (f_itr -> getCategory() == FacilityCategory::ECONOMY) {
            lastSelectedIndex = std::distance(start, f_itr);
            return *f_itr;
        }
    }
    // if there were no more economy type facilities left until the end of the vector it wraps back to the beginning of the vector to look for the next one
    for (start = facilitiesOptions.start; start != facilitiesOptions.end(); start++) {
        if (start -> getCategory() == FacilityCategory::ECONOMY) {
            lastSelectedIndex = std::distance(facilitiesOptions.begin(), start);
            return *start;
        }
    }
}

const string EconomySelection::toString() const override {
    return "eco";
}

EconomySelection* EconomySelection::clone() const override {
    return new EconomySelection(*this);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1) {}

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) override {
    lastSelectedIndex = (lastSelectedIndex++) % facilitiesOptions.size();
    vector<FacilityType>::iterator start = facilitiesOptions.begin() + lastSelectedIndex;
    // starts the iterator after the last time an enviroment type facility was chosen to and looks for the next one
    for (vector<FacilityType>::iterator f_itr = start; f_itr != facilitiesOptions.end(); f_itr++) {
        if (f_itr -> getCategory() == FacilityCategory::ENVIRONMENT) {
            lastSelectedIndex = std::distance(start, f_itr);
            return *f_itr;
        }
    }
    // if there were no more enviroment type facilities left until the end of the vector it wraps back to the beginning of the vector to look for the next one
    for (start = facilitiesOptions.begin(); start != facilitiesOptions.end(); start++) {
        if (start -> getCategory() == FacilityCategory::ENVIRONMENT) {
            lastSelectedIndex = std::distance(facilitiesOptions.begin(), start);
            return *start;
        }
    }
}

const string SustainabilitySelection::toString() const override {
    return "env";
}

SustainabilitySelection* SustainabilitySelection::clone() const override {
    return new SustainabilitySelection(*this);
}