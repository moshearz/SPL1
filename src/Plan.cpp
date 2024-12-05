#include "Plan.h"
#include "Facility.h"
#include <sstream>
#include <iostream>

//===============================================constructor==============================================
Plan::Plan(const int _planId, const Settlement& _settlement, SelectionPolicy* _selectionPolicy, const vector<FacilityType>& _facilityOptions) : plan_id(_planId), settlement(_settlement),
selectionPolicy(_selectionPolicy), status(PlanStatus::AVALIABLE), facilities(), underConstruction(),
facilityOptions(std::move(_facilityOptions)), life_quality_score(0), economy_score(0), environment_score(0) {}

//===========================================GETTERS================================================================
const int Plan::getlifeQualityScore() const { return life_quality_score;}

const int Plan::getEconomyScore() const { return economy_score;}

const int Plan::getEnvironmentScore() const { return environment_score;}

const int Plan::getPlanID() const { return plan_id;}

const vector<Facility*>& Plan::getFacilities() const {return facilities;}

//=============================================SETTERS====================================================
void Plan::setSelectionPolicy(SelectionPolicy* _selectionPolicy) {
    delete selectionPolicy;
    selectionPolicy = _selectionPolicy -> clone();
}


//============================================STEP=======================================================
void Plan::step() {
    //Stage 1: if the PlanStatus in "AVALIABLE" move to satge 2
     if (status == PlanStatus::AVALIABLE){
        //stage 2
        int constructonLimit = static_cast<int>(settlement.getType()) + 1;
        while (underConstruction.size() < constructonLimit) {
            addFacility(new Facility(selectionPolicy->selectFacility(facilityOptions), settlement.getName()));
        }
     }

     //Stage 3 (Now the PlanStatus is BUSY)
     std::vector<Facility*>::iterator iter;
     for (iter = underConstruction.begin(); iter != underConstruction.end();) {
        (*iter) -> Facility::step();
        if ((*iter) -> getStatus() == FacilityStatus::OPERATIONAL) {
            addFacility(*iter);
            iter = underConstruction.erase(iter);
        } else {iter++;}
     }
     //Stage 4
     if (underConstruction.size() == static_cast<int>(settlement.getType()) + 1) {
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
        << "\nEnviromentScore: " << environment_score;
        for (Facility* fa : facilityOrder) {
            oss << "\nFacilityName: " << fa -> getName()
            << "\nFacilityStatus: " << (fa -> getStatus() == FacilityStatus::OPERATIONAL ? "OPERATIONAL" : "UNDER CONSTRUCTION");
        }
    oss << "\n";
    std::cout << oss.str();
}



void Plan::addFacility(Facility* facility) {
    if (facility->getStatus() == FacilityStatus::UNDER_CONSTRUCTIONS) {
        underConstruction.push_back(facility);
        facilityOrder.push_back(facility);
    } else {
        facilities.emplace_back(facility);
        life_quality_score += facility->getLifeQualityScore();
        economy_score += facility->getEconomyScore();
        environment_score += facility->getEnvironmentScore();
    }
}

const string Plan::toString() const {
    std::ostringstream oss;
    oss << "PlanID: " << plan_id 
        << "\nSettlementName: " << settlement.getName()
        << "\nLifeQualityScore: " << life_quality_score
        << "\nEconomyScore: " << economy_score
        << "\nEnviromentScore: " << environment_score << "\n";
    return oss.str();
}


Plan::~Plan() {
    delete selectionPolicy;
    for (Facility* fp : facilities) {
        delete fp;
    }
    for (Facility* fp : underConstruction) {
        delete fp;
    }
}

Plan::Plan(const Plan& other) : plan_id(other.plan_id), settlement(other.settlement),
    status(other.status), facilityOptions(other.facilityOptions), life_quality_score(other.life_quality_score),
    economy_score(other.economy_score), environment_score(other.environment_score) {
    selectionPolicy = other.selectionPolicy->clone();
    for (Facility* fp : other.facilities) {
        facilities.push_back(new Facility(*fp));
    }
    for (Facility* fp : other.underConstruction) {
        underConstruction.push_back(new Facility(*fp));
    }
}

Plan::Plan(Plan&& other) : plan_id(other.plan_id), settlement(other.settlement), selectionPolicy(other.selectionPolicy),
    status(other.status), facilityOptions(other.facilityOptions), life_quality_score(other.life_quality_score),
    economy_score(other.economy_score), environment_score(other.environment_score) {
    other.selectionPolicy = nullptr;
    facilities = std::move(other.facilities);
    other.facilities.clear();
    underConstruction = std::move(other.underConstruction);
    other.underConstruction.clear();
}