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
    actionsLog.push_back(std::move(action));
}

bool Simulation::addSettlement(Settlement *settlement) {
    if (isSettlementExists(settlement -> getName())) return false;
    settlements.push_back(settlement);
    return true;
}

bool Simulation::addFacility(FacilityType Facility) {
    for (const FacilityType& ft : facilitiesOptions) {
        if (ft.getName() == Facility.getName()) {return false;}
    }
    facilitiesOptions.emplace_back(std::move(Facility));
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

    for (const Plan& p : plans) {
        std::cout << p.toString();
    }
    std::cout << "\nSimulation has been closed. Please come back :)" << std::endl;
}

void Simulation::open(){
    while(isRunning){

        std::string actionLine;
        std::getline(std::cin, actionLine);

        if (actionLine.empty()) {continue;}

        std::istringstream inputStreamm(actionLine);
        std::string command;
        inputStreamm >> command;

        if (command == "step"){
            int numOfSteps;
            inputStreamm >> numOfSteps;
            SimulateStep* current_act = new SimulateStep(numOfSteps);
            current_act -> act(*this);
            addAction(current_act);
        }
        else if (command == "plan"){
            std::string settlement_name, selection_policy;
            inputStreamm >> settlement_name >> selection_policy;

            AddPlan* current_act = new AddPlan(settlement_name, selection_policy);
            current_act -> act(*this);
            addAction(current_act);
        }
        else if (command == "settlement"){
            std::string settlement_name;
            int settlement_type;
            inputStreamm >> settlement_name >> settlement_type;

            AddSettlement* current_act = new AddSettlement(settlement_name, static_cast<SettlementType>(settlement_type));
            current_act -> act(*this);
            addAction(current_act);
        }
        else if (command == "facility"){

            std::string facility_name;
            int category, price, lifeq_impact, eco_impact, env_impact;
            inputStreamm >> facility_name >> category >> price >> lifeq_impact >> eco_impact >> env_impact;


            FacilityCategory category_enum = static_cast<FacilityCategory>(category);
         
            AddFacility* current_act = new AddFacility(facility_name, category_enum, price, lifeq_impact,eco_impact, env_impact);
            current_act -> act(*this);
            addAction(current_act);

        }
        else if (command == "planStatus"){
            int plan_id;
            inputStreamm >> plan_id;

            PrintPlanStatus* current_act = new PrintPlanStatus(plan_id);
            current_act -> act(*this);
            addAction(current_act);
        }
        else if (command == "changePolicy"){
            int plan_id;
            std::string selection_policy;
            inputStreamm >> plan_id >> selection_policy;

            ChangePlanPolicy* current_act = new ChangePlanPolicy(plan_id, selection_policy);
            current_act -> act(*this);
            addAction(current_act);
        }
        else if (command == "log") {PrintActionsLog().act(*this);}
        
        else if (command == "backup") {
            BackupSimulation* current_act = new BackupSimulation();
            current_act -> act(*this);
            addAction(current_act);
        }

        else if(command == "restore") {RestoreSimulation().act(*this);}

        else if (command == "close") {close();}

        else {std::cout << "Invalid method. Please try again." << std::endl;}
    }

}

bool Simulation::isPlanExists(const int& planId) const {
    vector<Plan>::const_iterator p_itr;
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

Simulation::~Simulation() {
    for (BaseAction* Act : actionsLog) {
        Act -> ~BaseAction();
    }
    for (Settlement* s : settlements) {
        s -> ~Settlement();
    }
}

Simulation::Simulation(const Simulation& other) : isRunning(other.isRunning), planCounter(other.planCounter) {
    for (BaseAction* Act : other.actionsLog) {
        actionsLog.push_back(Act -> clone());
    }
    for (const Plan& p : other.plans) {
        plans.push_back(Plan(p));
    }
    for (Settlement* s : other.settlements) {
        settlements.push_back(new Settlement(*s));
    }
    for (const FacilityType& ft : other.facilitiesOptions) {
        facilitiesOptions.push_back(FacilityType(ft));
    }
}