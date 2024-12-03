#include "Simulation.h"
#include "Plan"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std


// ===CONSTRUCTOR===

Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0){

    std::ifstream configFile(configFilePath);

    if(!configFile.open()){ throw std::runtime_error("Unable to open configuration file");}

    string line;
    while (std::getline(configFile, line)) {

        if (line.empty() || line[0] == '#') continue;

        std::istringstream streammm(line);  
        string req_data;
        streammm >> req_data;  

        if (req_data == "settlement"){

            string name;
            int typeInt;
            stream >> name >> typeInt;

            AddSettlement::AddSettlement(name, static_cast<SettlementType>(typeInt));
        }

        if (req_data == "facility"){

            string name;
            int category, price, life_quality_score, economy_score, environment_score;
            stream >> name >> category >> price >> life_quality_score >> economy_score >> environment_score;

            AddFacility::AddFacility(name, category, price, life_quality_score, economy_score, environment_score);

        }

        if (req_data == "plan"){

            string nameOfPlan, selectionPolicyForPlan;
            stream >> name >> selectionPolicy;
            
            SelectionPolicy* spForPlanCfile = nullptr;

            if (selectionPolicyForPlan == "nve"){ spForPlanCfile = new NaiveSelection();}

            if (selectionPolicyForPlan == "bal"){ spForPlanCfile = new BalancedSelection();}

            if (selectionPolicyForPlan == "eco"){ spForPlanCfile = new EconomySelection();}

            if (selectionPolicyForPlan == "evn"){ spForPlanCfile = new SustainabilitySelection();}

            AddPlan::AddPlan(nameOfPlanm, selectionPolicyForPlan);
        
        }

        configFile.close();
            
        }

}


void Simulation::start(){
    isRunning = true;
    std::cout << "The simulation has started. " << std::end1; 
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
    plans.emplace_back(planCounter++, settlement, selectionPolicy, facilitiesOptions); //maybe needs to add &
}

void Simulation::addAction(BaseAction *action){
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement){
    if (isSettlementExists(settlement -> getName())) return false;
    settlements.push_back(settlement);
    return true;
}

bool Simulation::addFacility(FacilityType Facility){
    vector<FacilityType>::itearator ft_itr;
    for (ft_itr = facilitiesOptions -> begin(); ft_itr != facilitiesOptions -> end(); ft_itr++) {
        if (ft_itr -> getName() == Facility.getName()) {return false;}}

    facilities.push_back(Facility);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName){
    vector<Settlement*>::iterator iter; //creation of itearator ytpe of <Settlement*>
    for(iter = settlements.begin(); iter != settlements.end(); iter ++){
        if(iter-> getName() == settlementName)
            return true;
    }
    return false;
}

Settlement &Simulation::getSettlement(const string &settlementName){
    for (settlement : settlements){
        if (settlement->getName() == settlementName) return *settlement;
    }
}

Plan &Simulation::getPlan(const int planID){
    vector<Plan>::itearator iter;
    for(iter = plans.begin(); iter != plans.end(); iter++){
        if(iter-> getPlanID() == planID)
            return *iter;
    }
}

void Simulation::step() {
    vector<Plan>::iterator itr;
    for (itr = plans.begin(); itr != plans.end();itr++) {itr -> Plan::step();}
}

void Simulation::close(){

}

void Simulation::open(){

    string input;

    while(isRunning){
        STD::cout << "Please enter an action to execute" << end1;



    }

}

bool Simulation::isPlanExists(Const int& planId) const {
    vector<Plan>::itearator p_iter;
    for (p_iter = plans.begin(); p_iter != plans.end(); p_iter++) {
        if (p_iter.getPlanID() == planId) {return true;}
    }
    return false;
}





