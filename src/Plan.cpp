#include "Plan.h"
#include "Facility.h"

//===============================================constructor==============================================
Plan::Plan(const int _planId, const Settlement& _settlement, SelectionPolicy* _selectionPolicy, const vector<FacilityType>& _facilityOptions) : life_quality_score(0), economy_score(0),
        environment_score(0), plan_id(_planId), settlement(_settlement), selectionPolicy(_selectionPolicy), status(PlanStatus::AVALIABLE), facilityOptions(_facilityOptions) {}

//===========================================GETTERS================================================================
const int Plan::getLifeQualityScore() const { return life_quality_score;}

const int Plan::getEconomyScore() const { return economy_score;}

const int Plan::getEnvironmentScore() const { return environment_score;}

const int Plan::getPlanID() const { return plan_id;}

const vector<Facility*>& Plan::getFacilities() const {return facilities;}

//=============================================SETTERS====================================================
void Plan::setSelectionPolicy(SelectionPolicy* _selectionPolicy) {
    *selectionPolicy = *_selectionPolicy; //maybe add a copy/move assignment operator
}


//============================================STEP=======================================================
void Plan::step() {
    //Stage 1: if the PlanStatus in "AVALIABLE" move to satge 2
     if (status == PlanStatus::AVALIABLE){
        //stage 2
        int constructonLimit = static_cast<int>(settlement.getType()) + 1;
        while (underConstruction.size() < constructonLimit){
            addFacility(new Facility(selectionPolicy->selectFacility(facilityOptions)), settlement.getName());
        }
     }

     //Stage 3 (Now the PlanStatus is BUSY)
     std::vector<Facility*>::itearator iter;
     for (iter=underConstruction.begin(); iter != underConstruction.end();) {
            iter->Facility::step();
            vector<Facility*>* cur_facility = iter;
            iter++
            addFacility(cur_facility);
     }
     //Stage 4
     if (underConstruction.size() == static_cast<int>(settlement.getType() + 1)) {
        status = PlanStatus::BUSY;}
    else {status = PlanStatus::AVALIABLE;}
}

void Plan::printStatus() {
    std::ostringstream oss;
    oss << "PlanID: " << plan_id 
        << "\nSettlementName: " << settlement.getName()
        << "\nPlanStatus: " << (status == PlanStatus::BUSY ? "BUSY" : "AVAILIABLE")
        << "\nSelectionPolicy: " << selectionPolicy -> toString()
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
    cout << oss.str();
}



void Plan::addFacility(Facility* facility) {
    if (facility->getStatus() == FacilityStatus::UNDER_CONSTRUCTIONS) {underConstruction.push_back(facility);}
    else { // FacilityStatus::OPERATIONAL
        underConstruction.erase(facility);
        facilities.push_back(facility);
    }
}

const string Plan::toString() const {
    return "Plan ID: " << plan_id << settlement << *selectionPolicy << "Status: " << status << "Score: " << life_quality_score << " " << economy_score << " " << environment_score;
}

SelectionPolicy* Plan::createSelectionPolicy(const string& _selectionPolicy) const {
    if (_selectionPolicy == "nve") {return new NaiveSelection::NaiveSelection();}
    else if (_selectionPolicy == "bal") {return new BalancedSelection::BalancedSelection(life_quality_score, economy_score, environment_score);}
    else if (_selectionPolicy == "eco") {return new EconomyScore::EconomyScore();}
    else {return new SustainabilitySelection::SustainabilitySelection(;)}
}

void printFinalStatus() const {
    std::ostringstream oss;
    oss << "PlanID: " << plan_id 
        << "\nSettlementName: " << settlement.getName()
        << "\nSelectionPolicy: " << selectionPolicy -> toString()
        << "\nLifeQualityScore: " << life_quality_score
        << "\nEconomyScore: " << economy_score
        << "\nEnviromentScore: " << environment_score
    cout << oss.str();

    delete selectionPolicy;
    for (Facility* fa : facilities) {delete fa;}
    for (Facility* uc_fa : underConstruction) {delete uc_fa;}
}