#include "Simulation.h"
#include "Action.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>

// ===============================================CONSTRUCTOR=================================================
Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0),
actionsLog(), plans(), settlements(), facilitiesOptions() {
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

            settlements.push_back(new Settlement(name, static_cast<SettlementType>(typeInt)));
        }
        else if (req_data == "facility"){
            string name;
            int category, price, life_quality_score, economy_score, environment_score;
            streamm >> name >> category >> price >> life_quality_score >> economy_score >> environment_score;

            FacilityType curr_ft = FacilityType(name, static_cast<FacilityCategory>(category), price, life_quality_score, economy_score, environment_score);
            facilitiesOptions.push_back(curr_ft);
        }
        else if (req_data == "plan"){
            string nameOfsettlement, nameOfselectionPoliy;
            streamm >> nameOfsettlement >> nameOfselectionPoliy;

            plans.emplace_back(planCounter++, getSettlement(nameOfsettlement), createSelectionPolicy(nameOfselectionPoliy, 0, 0, 0), facilitiesOptions);
        }            
    }

    configFile.close();
}

// ===============================================METHODS IMPLEMANTIONS=================================================
void Simulation::start(){
    isRunning = true;
    std::cout << "The simulation has started.\n";
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
    
    // Should never reach here since this function gets called only if isSettlementExists passes or
    // its being called in the simulation constructor which is guaranteed to pass
    std::terminate(); // Used to supress the flag warnings 
}

Plan& Simulation::getPlan(const int planID){
    vector<Plan>::iterator iter;
    for(iter = plans.begin(); iter != plans.end(); iter++){
        if(iter->getPlanID() == planID)
            return *iter;
    }

    // Should never reach here since this function gets called only if isPlanExists passes
    std::terminate(); // Used to supress the flag warnings 
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
    for (Settlement* s : settlements) {
        if (s) {delete s;}
    }
    std::cout << "\nSimulation has been closed. Please come back :)" << std::endl;
}

void Simulation::open(){
    while(isRunning){

        std::string actionLine;
        std::getline(std::cin, actionLine);

        if (actionLine.empty()) {continue;}
        std::istringstream inputStreamm(actionLine);
        std::system("clear");
        std::string command;
        inputStreamm >> command;

        if (command == "step"){
            int numOfSteps;
            inputStreamm >> numOfSteps;

            (new SimulateStep(numOfSteps)) -> act(*this);
        }
        else if (command == "plan"){
            std::string settlement_name, selection_policy;
            inputStreamm >> settlement_name >> selection_policy;

            (new AddPlan(settlement_name, selection_policy)) -> act(*this);
        }
        else if (command == "settlement"){
            std::string settlement_name;
            int settlement_type;
            inputStreamm >> settlement_name >> settlement_type;

            (new AddSettlement(settlement_name, static_cast<SettlementType>(settlement_type))) -> act(*this);
        }
        else if (command == "facility"){

            std::string facility_name;
            int category, price, lifeq_impact, eco_impact, env_impact;
            inputStreamm >> facility_name >> category >> price >> lifeq_impact >> eco_impact >> env_impact;

            FacilityCategory category_enum = static_cast<FacilityCategory>(category);
            (new AddFacility(facility_name, category_enum, price, lifeq_impact,eco_impact, env_impact)) -> act(*this);
        }
        else if (command == "planStatus"){
            int plan_id;
            inputStreamm >> plan_id;

            (new PrintPlanStatus(plan_id)) -> act(*this);
        }
        else if (command == "changePolicy"){
            int plan_id;
            std::string selection_policy;
            inputStreamm >> plan_id >> selection_policy;

            (new ChangePlanPolicy(plan_id, selection_policy)) -> act(*this);
        }
        else if (command == "log") {PrintActionsLog().act(*this);}
        
        else if (command == "backup") {(new BackupSimulation()) -> act(*this);}

        else if(command == "restore") {(new RestoreSimulation()) -> act(*this);}

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
        std::cout << actionLine -> toString() << "\n";}
}

SelectionPolicy* Simulation::createSelectionPolicy(const string& _selectionPolicy, int _life_quality_score, int _economy_score, int _enviroment_score) const {
    if (_selectionPolicy == "nve") {return new NaiveSelection();}
    else if (_selectionPolicy == "bal") {return new BalancedSelection(_life_quality_score, _economy_score, _enviroment_score);}
    else if (_selectionPolicy == "eco") {return new EconomySelection();}
    else {return new SustainabilitySelection();}
}

vector<Settlement*>& Simulation::getSettlements() {return settlements;}

Simulation::~Simulation() {// since settlements is shared between simulation and the backup we will manually delete it at the end of the program
    for (BaseAction* Act : actionsLog) {
        if (Act) {delete Act;}
    }
}

Simulation::Simulation(const Simulation& other) : isRunning(other.isRunning), planCounter(other.planCounter),
actionsLog(), plans(), settlements(other.settlements), facilitiesOptions() {
    for (BaseAction* Act : other.actionsLog) {
        actionsLog.push_back(Act -> clone());
    }
    for (const Plan& p : other.plans) {
        plans.push_back(Plan(p));
    }
    for (const FacilityType& ft : other.facilitiesOptions) {
        facilitiesOptions.push_back(FacilityType(ft));
    }
}

Simulation::Simulation(Simulation&& other) : isRunning(other.isRunning), planCounter(other.planCounter),
actionsLog(std::move(other.actionsLog)), plans(std::move(other.plans)), settlements(std::move(other.settlements)), 
facilitiesOptions(std::move(other.facilitiesOptions)) {}
///fdvdv