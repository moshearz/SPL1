#pragma once
#include <vector>
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

        PlanStatus getPlanStatus() const;
        selectionPolicy& getSelecetionPolicy() const; //add const
        vector<Facility*>& getUnderConstruction() const;
        Settlement& getSettlement() const;
        void setPlanStatus(); //updates the plan status depending on the current underConstruction list size
        const int getPlanID() const;
        SelectionPolicy* createSelectionPolicy(const string& selectionPolicy) const;
        

    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};