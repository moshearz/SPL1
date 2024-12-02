#include "Plan.h"
#include "Facility.h"


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

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy) {
    if (selectionPolicy) // equal to: if (selectionPolicy != null)
        delete selectionPolicy 
    selectionPolicy = new selectionPolicy
}



void Plan::setPlanStatus(){

}

void Plan::step(){
    //Stage 1: if the PlanStatus in "AVALIABLE" move to satge 2
     if (status == PlanStatus::AVALIABLE){
        //stage 2
        int constructonLimit = settlement.getConstructionLimit();
        while (underConstruction.size() < constructonLimit && !facilitiesOptions.empty()){
            FacilityType& nextFacilityTypeToBuild = selectionPolicy->selectFacility(facilityOptions);
            Facility *newFacility = new Facility(nextFacilityTypeToBuild, settlement.getName());
            underConstruction.push_back(newFacility);
        }
     }

     //Stage 3 (Now the PlanStatus is BUSY)
     std::vector<Facility*>::itearator iter;
     for (iter = underConstruction.begin(); iter != underConstruction.end();){
            FacilityStatus currentUnderConstructionFacility = iter->step();  // Call step() to decrement timeLeft
            if(currentUnderConstructionFacility == FacilityStatus::OPERATIONAL){
                facilities.push_back(iter);  // Move to operational facilities
                iter = underConstruction.erase(iter);  // Remove from underConstruction
            } else {
                iter++;
            }
     }
     //Stage 4
     if (underConstruction.size() == settlement.getConstructionLimit())
        status = PlanStatus::BUSY;
    else
        status = PlanStatus::AVALIABLE;
}

