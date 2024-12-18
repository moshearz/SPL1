#pragma once
#include <vector>
#include <sstream>
#include <iostream>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility); //addFacility removes said facility from underConstruction and moves it to Facilites
        const string toString() const;


        const int getPlanID() const;
        string getSelectionPolicy() const;

        ~Plan(); //Required to clean up SelectionPolicy* and the std::vector<Facility*> elements.
        Plan(const Plan& other); //Required to deep copy SelectionPolicy* (via a clone method) and deep copy facilities and underConstruction vectors
        Plan(Plan&& other); //Required to transfer ownership of SelectionPolicy* and vectors.
        Plan& operator=(const Plan& other) = delete;
        Plan& operator=(Plan&& other) = delete;        

    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;

        vector<Facility*> facilityOrder;
};