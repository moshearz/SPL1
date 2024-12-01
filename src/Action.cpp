#include "Action.h"
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;


ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() { //check if 0 or COMPLETED
    status = 0;
}

void BaseAction::error(string _errorMsg) {
    errorMsg = _errorMsg;
}

const string &BaseAction::getErrorMsg() const {
    return errorMsg;
}

SimulateStep::SimulateStep(const int _numOfSteps) : numOfSteps(_numOfSteps) {}

void SimulateStep::act(Simulation &simulation) {
    for (i = 0; i < numOfSteps; i++) {
        vector<Plan> pl = simulation.getPlanList();
        vector<Plan>::iterator itr;
        for (itr = pl.begin(); itr != pl.end();) {
            if (itr -> getPlanStatus() == PlanStatus::AVALIABLE) { //maybe cant use PlanStatus directly
                (itr -> underConstruction).emplace_back((itr -> getSelecetionPolicy()).selectFacility(simulation.getFacilityOptions()));
            } else {
                vector<Facility*> ucl = itr -> getUnderConstruction();
                vector<Facility*>::iterator itr_ucl;
                for (itr_ucl = ucl.begin(); itr_ucl != ucl.end(); itr_ucl++) {
                    itr_ucl -> step();
                    if (itr_ucl -> getStatus() == FacilityStatus::OPERATIONAL) {
                        itr -> addFacility(itr_ucl);
                    }
                }
                itr++;
            }
            itr -> setPlanStatus();
        }
    }
}

const string SimulateStep::toString() const {
    return "The number of steps is: " + numOfSteps;
}

SimulateStep *SimulateStep::clone() const {
    return new SimulateStep(*this);
}

AddPlan::AddPlan(const string &_settlementName, const string &_selectionPolicy) : settlementName(_settlementName), selectionPolicy(_selectionPolicy) {}

void AddPlan::act(Simulation &simulation) {
    vector<Settlement*>::iterator iter;
    for (iter = simulation)
}