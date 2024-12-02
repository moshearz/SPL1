#include "Plan.h"
#include "Facility.h"


Plan::Plan(const int _planId, const Settlement& _settlement, SelectionPolicy* _selectionPolicy, const vector<FacilityType>& _facilityOptions) : life_quality_score(0), economy_score(0),
        environment_score(0), plan_id(_planId), settlement(_settlement), selectionPolicy(_selectionPolicy), status(PlanStatus::AVALIABLE), facilityOptions(_facilityOptions) {}

//First lets starts with all the getters 

const int Plan::getLifeQualityScore() const { return life_quality_score;}

const int Plan::getEconomyScore() const { return economy_score;}

const int Plan::getEnvironmentScore() const { return environment_score;}

void Plan::setSelectionPolicy(SelectionPolicy* _selectionPolicy) {
    if (_selectionPolicy.getType() == selectionPolicy.getType()) {Action::error("Cannot change selection policy");}
    else {*selectionPolicy = *_selectionPolicy;}
}

void Plan::step() {
    //Stage 1: if the PlanStatus in "AVALIABLE" move to satge 2
     if (status == PlanStatus::AVALIABLE){
        //stage 2
        int constructonLimit = settlement.getConstructionLimit();
        while (underConstruction.size() < constructonLimit){
            addFacility(new Facility(selectionPolicy->selectFacility(facilityOptions)), settlement.getName());
        }
     }

     //Stage 3 (Now the PlanStatus is BUSY)
     std::vector<Facility*>::itearator iter;
     for (iter = underConstruction.begin(); iter != underConstruction.end(); iter++) {
            // FacilityStatus currentUnderConstructionFacility = iter -> Facility::step();  // Call step() to decrement timeLeft
            // if(currentUnderConstructionFacility == FacilityStatus::OPERATIONAL){
            //     facilities.push_back(iter);  // Move to operational facilities
            //     iter = underConstruction.erase(iter);  // Remove from underConstruction
            // } else {
            //     iter++;
            // }
            iter -> Facility::step();
            addFacility(iter);
     }
     //Stage 4
     if (underConstruction.size() == settlement.getConstructionLimit()) {
        status = PlanStatus::BUSY;}
    else {status = PlanStatus::AVALIABLE;}
}

void Plan::printStatus() {
    std::ostringstream oss;
    oss << "PlanID: " << plan_id 
        << "\nSettlementName: " << settlement.getName()
        << "\nPlanStatus: " << (status == PlanStatus::BUSY ? "BUSY" : "AVAILIABLE")
        << "\nSelectionPolicy: " << selectFacility.getType()
        << "\nLifeQualityScore: " << life_quality_score
        << "\nEconomyScore: " << economy_score
        << "\nEnviromentScore: " << environment_score
        for (Facility* fa : facilities) {
            << "\nFacilityName: " << fa -> getName();
            << "\nFacilityStatus: " << "OPERATIONAL";
        }
        for (Facility* fa : underConstruction) {
            << "\nFacilityName: " << fa -> getName();
            << "\nFacilityStatus: " << "UNDER CONSTRUCTION";
        }
    return oss.str();
}

const vector<Facility*>& Plan::getFacilities() const {return facilities;}

void Plan::addFacility(Facility* facility) {
    if (facility -> getStatus() == FacilityStatus::UNDER_CONSTRUCTIONS) {underConstruction.push_back(facility);}
    else { // FacilityStatus::OPERATIONAL
        underConstruction.erase(facility);
        facilities.push_back(facility);
    }
}

const string toString() const {
    return "Plan ID: " + plan_id + settlement + *selectionPolicy + "Status: " + status + "Score: " + life_quality_score + " " + economy_score + " " + environment_score;
}

selectionPolicy& Plan::getSelecetionPolicy() const { return *selectionPolicy;}


//Now, lets move to all the setters 
