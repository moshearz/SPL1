#include "Simulation.h"
#include "Plan"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std


// ===CONSTRUCTOR===

Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0) {

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
            
            SelectionPolicy* spForPlanCfile = Plan::createSelectionPolicy(selectionPolicy);

            AddPlan::AddPlan(nameOfPlanm, selectionPolicyForPlan);
        
        }

        configFile.close();
            
        }

}


void Simulation::start(){
    isRunning = true;
    std::cout << "The simulation has started. " << std::end1;
    open();
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy) {
    plans.emplace_back(planCounter++, settlement, selectionPolicy, facilitiesOptions); //maybe needs to add &
}

void Simulation::addAction(BaseAction *action) {
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement) {
    if (isSettlementExists(settlement -> getName())) return {false;}
    settlements.push_back(settlement);
    return true;
}

bool Simulation::addFacility(FacilityType Facility) {
    vector<FacilityType>::itearator ft_itr;
    for (ft_itr = facilitiesOptions -> begin(); ft_itr != facilitiesOptions -> end(); ft_itr++) {
        if (ft_itr -> getName() == Facility.getName()) {return false;}}

    facilitiesOptions.push_back(Facility);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName) {
    vector<Settlement*>::iterator iter; //creation of itearator type of <Settlement*>
    for (iter = settlements.begin(); iter != settlements.end(); iter ++){
        if(iter-> getName() == settlementName)
            return true;
    }
    return false;
}

Settlement& Simulation::getSettlement(const string &settlementName) {
    for (settlement : settlements){
        if (settlement->getName() == settlementName) return *settlement;}
}

Plan& Simulation::getPlan(const int planID){
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

void Simulation::close() {
    isRunning = false;

    for (vector<Plan>::iterator p_itr = plans.begin(); p_itr != plans.end(); p_itr++) {
        p_itr -> printFinalStatus();}
    
    for (vector<BaseAction*>::iterator ac_itr = actionsLog.begin(); ac_itr != actionsLog.end(); ac_itr++) {
        delete *ac_itr;}
    
    for (vector<Settlement*>::iterator s_itr = settlements.begin(); s_itr != settlements.end(); s_itr++) {
        delete *s_itr;}
}

void Simulation::open(){

    string input;

    while(isRunning){

        std::cout << "Please enter an action to execute" << end1;
        std::string actionLine;
        std::getline(std::cin, std::actionLine);

        if (actionLine.empty()) continue;

        std::istringstream inputStreamm(actionLine);
        std::sring command;
        inputStreamm >> command;

        if (inputStreamm == "step"){

            int numOfSteps;
            inputStreamm >> numOfSteps;
            
            //if(inputStreamm.fail() || numOfSteps <= 0). "Thia action never results an error. we can assume numOfStep is always a number"

            SimulateStep::SimulateStep(3).act(*this); // this is pointer to the current object(simulation)

            // this is an lvalue, but we do not need this action afterwards so we will use the upperline.
            //SimulateStep stepFromAction = SimulateStep(3);
           // stepFromAction.act(this);
        }

        else if (inputStreamm == "plan"){

            std::sring settlement_name, selection_policy;
            inputStreamm >> settlement_name >> selection_policy;

            AddPlan::AddPlan(settlement_name, selection_policy).act(*this);
        }

        else if (inputStreamm == "settlement"){

            std::string settlement_name, settlement_type;
            inputStreamm >> settlement_name >> settlement_type;

            AddSettlement::AddSettlement(settlement_name, settlement_type).act(*this);
        }

        else if (inputStreamm == "facillity"){

            std::string facillity_name;
            int category, price, lifeq_impact, eco_impact, env_impact;
            inputStreamm >> facillity_name >> category >> price >> lifeq_impact >> eco_impact >> env_impact;

            if (category == 0){
                FacilityCategory category_convertFromInt = LIFE_QUALITY;
            }
            else if (category == 1){
                FacilityCategory category_convertFromInt = ECONOMY;
            
            }
            else (category == 2){
                FacilityCategory category_convertFromInt = ENVIRONMENT;
            }

            AddFacility::AddFacility(facillity_name, category_convertFromInt, price, lifeq_impact,eco_impact,eco_impact, env_impact).act(*this);

        }

        else if (inputStreamm == "planStatus"){

            int plan_id;
            inputStreamm >> plan_id;

            PrintPlanStatus::PrintPlanStatus(plan_id).act(*this);
        }

        else if (inputStreamm == "changePolicy"){

            int plan_id;
            std::string selection_policy;
            inputStreamm >> plan_id >> selection_policy;

            ChangePlanPolicy::ChangePlanPolicy(plan_id, selection_policy).act(*this);
        }

        else if













    }

}

bool Simulation::isPlanExists(Const int& planId) const {
    vector<Plan>::itearator p_iter;
    for (p_iter = plans.begin(); p_iter != plans.end(); p_iter++) {
        if (p_iter -> getPlanID() == planId) {return true;}
    }
    return false;
}