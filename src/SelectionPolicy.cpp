#include "SelectionPolicy.h"
#include <iostream>
#include <cmath>
using namespace std;


NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
    //add empty facilitiesOptions verctor case
}

const string NaiveSelection::toString() {
    return "Naive Selection Policy Selected";
}

NaiveSelection * NaiveSelection::clone() const {
    return new NaiveSelection(*this);
}

NaiveSelection::~NaiveSelection() {if (this) delete this;}

BalancedSelection::BalancedSelection(int _LifeQualityScore, int _EconomyScore, int _EnvironmentScore) : LifeQualityScore(_LifeQualityScore), EconomyScore(_EconomyScore), EnvironmentScore(_EnvironmentScore) {}
const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    int selected;
    int selected_distance;
    for (int i = 0; i < facilitiesOptions.size(); i++) {
        int distance = 0;
        int life_eco = abs(LifeQualityScore + facilitiesOptions[i].getLifeQualityScore() - EconomyScore - facilitiesOptions[i].getEconomyScore());
        int life_env = abs(LifeQualityScore + facilitiesOptions[i].getLifeQualityScore() - EnvironmentScore - facilitiesOptions[i].getEnvironmentScore());
        int eco_env = abs(EconomyScore + facilitiesOptions[i].getEconomyScore() - EnvironmentScore - facilitiesOptions[i].getEnvironmentScore());
        if (life_eco > life_env) {
            if (life_eco > eco_env) distance = life_eco;
            else distance = eco_env;
        }
        else {
            if (life_env > eco_env) distance = life_env;
            else distance = eco_env;
        }
        if (!selected) {
            selected = i;
            selected_distance = distance;
        } else {
            if (distance < selected_distance) {
                selected = i;
                selected_distance = distance;
            }
        }
    }
    return facilitiesOptions[selected]; //add null case
}

const string BalancedSelection::toString() {
    return "Balanced Selection Policy Selected";
}

BalancedSelection * BalancedSelection::clone() const {
    return new BalancedSelection(*this);
}

BalancedSelection::~BalancedSelection() {if (this) delete this;}

EconomySelection::EconomySelection()