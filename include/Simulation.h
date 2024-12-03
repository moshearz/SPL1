#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy); //the selection policy pointer points to memory in heap
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement); //the settlement pointer points to memory in heap
        bool addFacility(FacilityType facility); //the facility pointer points to memory in heap
        bool isSettlementExists(const string &settlementName);
        Settlement &getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
        void step();
        void close();
        void open();

        bool isPlanExists(const int& planId) const;
        void printActionsLog() const;
        SelectionPolicy* Simulation::createSelectionPolicy(const string& _selectionPolicy, int _life_quality_score, int _economy_score, int _enviroment_score) const;

    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;
};