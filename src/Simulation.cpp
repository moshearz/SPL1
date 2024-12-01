#include "Simulation.h"
#include "Plan"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std

Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0){
    ifstream configFile(configFilePath);
    if(!configFile.open()){
        cout << "Failed to open"
    }



}

void Simulation::start(){
    isRunning = true;
    std::cout << "Simulation started. " << std::end1; 
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
    plans.emplace_back(planCounter++, settlement, selectionPolicy, facilitiesOptions); //maybe needs to add &
}

void Simulation::addAction(BaseAction *action){
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement){
    // equals to: if (isSettlementExists((*settlement).getName())
    //if (isSettlementExists(settlement -> getName())) return false;
    settlements.push_back(settlement);
    return true;
}

bool Simulation::addFacility(FacilityType Facility){
    facilities.push_back(Facility);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName){
    vector<Settlement*>::iterator iter; //creation of itearator ytpe of <Settlement*>
    for(iter = settlements.begin(); iter != settlements.end(); iter ++){
        if(iter-> getName() == settlementName)
            return false;
    }
    return true;
}

Settlement &Simulation::getSettlement(const string &settlementName){
    for (settlement : settlements){
        if (settlement->getName() == settlementName) return *settlement;
    }
}

Plan &Simulation::getPlan(const int planID){
    vector<Plan>::itearator iter;
    for(iter = plans.begin(); iter != plans.end(); iter++){
        if(iter->plan_id == planID)
            return plan
    }
  
}







