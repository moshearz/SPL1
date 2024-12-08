#include "SelectionPolicy.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    lastSelectedIndex++;
    return facilitiesOptions[lastSelectedIndex % facilitiesOptions.size()];
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
        int result = std::max(std::abs(life - eco), std::max(std::abs(eco - env), std::abs(life - env)));
        if (result < distance) {
            selected = &(*itr);
            distance = result;
        }
    }
    LifeQualityScore += selected -> getLifeQualityScore();
    EconomyScore += selected -> getEconomyScore();
    EnvironmentScore += selected -> getEnvironmentScore();
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
    do {
        lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    } while (facilitiesOptions[lastSelectedIndex].getCategory() != FacilityCategory::ECONOMY);
    return facilitiesOptions[lastSelectedIndex];
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
    do {
        lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    } while (facilitiesOptions[lastSelectedIndex].getCategory() != FacilityCategory::ENVIRONMENT);
    return facilitiesOptions[lastSelectedIndex];
}

const string SustainabilitySelection::toString() const {
    return "env";
}

SustainabilitySelection* SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this);
}