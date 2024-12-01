#include "Plan.h"

Plan::Plan(const int planID, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector <FacilityType> &facilitiesOptions) : life_quality_score(0), economy_score(0), environment_score(0) 
    : plan_id(plan_id), settlement(settlement), selectionPolicy(selectionPolicy), status(status), facilities(facilities), underConstruction(underConstruction), facilityOptions(facilitiesOptions) {}



//First lets starts with all the getters 

const int Plan::getLifeQualityScore() const { return life_quality_score}

const int Plan::getEconomyScore() const { return economy_score}

const int Plan::getEnvironmentScore() const { return environment_score}

PlanStatus Plan::getPlanStatus() const { return status}

selectionPolicy Plan::getSelecetionPolicy() const { return *selectionPolicy}

vector<Facility*>& Plan::getUnderConstruction() const { return underConstruction}

Settlement& Plan::getSettlement() const {return settlement}

//Now, lets move to all the setters 

void Plan:: setSelectionPolicy(SelectionPolicy *selectionPolicy) {
    selectionPolicy = new selectionPolicy

}

