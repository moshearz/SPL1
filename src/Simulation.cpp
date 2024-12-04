#include "Simulation.h"
#include "Plan.h"
#include "Action.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;
// VERIFY RULE OF 5 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// ===============================================CONSTRUCTOR=================================================
Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0) {
    std::ifstream configFile(configFilePath);

    string line;
    while (std::getline(configFile, line)){

        if (line.empty() || line[0] == '#') continue;

        std::istringstream streamm(line);  
        string req_data;
        streamm >> req_data;  

        if (req_data == "settlement"){

            string name;
            int typeInt;
            streamm >> name >> typeInt;

            AddSettlement(name, static_cast<SettlementType>(typeInt)).act(*this);
        }
        else if (req_data == "facility"){
            string name;
            int category, price, life_quality_score, economy_score, environment_score;
            streamm >> name >> category >> price >> life_quality_score >> economy_score >> environment_score;

            AddFacility(name, static_cast<FacilityCategory>(category), price, life_quality_score, economy_score, environment_score).act(*this);
        }
        else if (req_data == "plan"){
            string nameOfsettlement, nameOfselectionPoliy;
            streamm >> nameOfsettlement >> nameOfselectionPoliy;

            AddPlan(nameOfsettlement, nameOfselectionPoliy).act(*this);
            //maybe we need toclean up dynamically allocated memory ---> delete selectionPolicy ??????? Rule of 5 ????? 
        }            
    }

    configFile.close();
}

// ===============================================METHODS IMPLEMANTIONS=================================================
void Simulation::start(){
    isRunning = true;
    std::cout << "The simulation has started. " << std::endl;
    open();
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy) {
    plans.emplace_back(planCounter++, settlement, selectionPolicy, facilitiesOptions); 
}

void Simulation::addAction(BaseAction *action) {
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement) {
    if (isSettlementExists(settlement -> getName())) return false;
    settlements.push_back(settlement);
    return true;
}

bool Simulation::addFacility(FacilityType Facility) {
    vector<FacilityType>::iterator ft_itr;
    for (ft_itr = facilitiesOptions.begin(); ft_itr != facilitiesOptions.end(); ft_itr++) {
        if (ft_itr->getName() == Facility.getName()) return false;
    }
    facilitiesOptions.push_back(Facility);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName) {
    vector<Settlement*>::iterator iter; 
    for (iter = settlements.begin(); iter != settlements.end(); iter ++){
        if((*iter)->getName() == settlementName)
            return true;
    }
    return false;
}

Settlement& Simulation::getSettlement(const string &settlementName) {
    for (Settlement* settlement : settlements){
        if (settlement->getName() == settlementName) return *settlement;}
}

Plan& Simulation::getPlan(const int planID){
    vector<Plan>::iterator iter;
    for(iter = plans.begin(); iter != plans.end(); iter++){
        if(iter->getPlanID() == planID)
            return *iter;
    }
}

void Simulation::step() {
    vector<Plan>::iterator iter;
    for(iter=plans.begin(); iter != plans.end(); iter++)
        iter->Plan::step();
}

void Simulation::close() {
    isRunning = false;

    for (vector<Plan>::iterator p_itr = plans.begin(); p_itr != plans.end(); p_itr++) {
        p_itr -> printFinalStatus();}
    
    for (vector<BaseAction*>::iterator ac_itr = actionsLog.begin(); ac_itr != actionsLog.end(); ac_itr++) {
        delete *ac_itr;}
    
    for (vector<Settlement*>::iterator s_itr = settlements.begin(); s_itr != settlements.end(); s_itr++) {
        delete *s_itr;}

    std::cout << "Simulation has been closed. Please come back :)" << std::endl;
}

void Simulation::open(){
    while(isRunning){
        std::cout << "Please enter an action to execute" << std::endl;

        std::string actionLine;
        std::getline(std::cin, actionLine);

        if (actionLine.empty()) continue;

        std::istringstream inputStreamm(actionLine);
        std::string command;
        inputStreamm >> command;

        if (command == "step"){
            int numOfSteps;
            inputStreamm >> numOfSteps;
            //if(inputStreamm.fail() || numOfSteps <= 0). "Thia action never results an error. we can assume numOfStep is always a number"
            SimulateStep(numOfSteps).act(*this); // this is pointer to the current object(simulation)
        }
        else if (command == "plan"){
            std::string settlement_name, selection_policy;
            inputStreamm >> settlement_name >> selection_policy;

            AddPlan(settlement_name, selection_policy).act(*this);
        }
        else if (command == "settlement"){
            std::string settlement_name, settlement_type;
            inputStreamm >> settlement_name >> settlement_type;

            AddSettlement(settlement_name, settlement_type).act(*this);
        }
        else if (command == "facility"){

            std::string facility_name;
            int category, price, lifeq_impact, eco_impact, env_impact;
            inputStreamm >> facility_name >> category >> price >> lifeq_impact >> eco_impact >> env_impact;


            FacilityCategory category_enum = static_cast<FacilityCategory>(category);
         
            AddFacility(facility_name, category_enum, price, lifeq_impact,eco_impact,eco_impact, env_impact).act(*this);

        }
        else if (command == "planStatus"){
            int plan_id;
            inputStreamm >> plan_id;

            PrintPlanStatus(plan_id).act(*this);
        }
        else if (command == "changePolicy"){
            int plan_id;
            std::string selection_policy;
            inputStreamm >> plan_id >> selection_policy;

            ChangePlanPolicy(plan_id, selection_policy).act(*this);
        }
        else if (command == "log") {PrintActionsLog().act(*this);}
        
        else if (command == "backup") {BackupSimulation().act(*this);}

        else if(command == "restore") {RestoreSimulation().act(*this);}

        else if (command == "close") {close();}

        else {std::cout << "Invalid method. Please try again." << std::endl;}

    }

}

bool Simulation::isPlanExists(const int& planId) const {
    vector<Plan>::iterator p_itr;
    for (p_itr = plans.begin(); p_itr != plans.end(); p_itr++) {
        if (p_itr->getPlanID() == planId) 
            return true;
    }
    return false;
}

void Simulation::printActionsLog() const{
    for (BaseAction* actionLine : actionsLog) {
        cout << actionLine -> toString() << "\n";}
}

SelectionPolicy* Simulation::createSelectionPolicy(const string& _selectionPolicy, int _life_quality_score, int _economy_score, int _enviroment_score) const {
    if (_selectionPolicy == "nve") {return new NaiveSelection();}
    else if (_selectionPolicy == "bal") {return new BalancedSelection(_life_quality_score, _economy_score, _enviroment_score);}
    else if (_selectionPolicy == "eco") {return new EconomySelection();}
    else {return new SustainabilitySelection();}
}