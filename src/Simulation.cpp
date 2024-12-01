#include "Simulation.h"
#include "Plan"

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
    settlement &settlement = getSettlement(settlementName);
    plans.emplace_back(planCounter++, settlement, selectionPolicy, facilitiesOptions)
}

void Simulation