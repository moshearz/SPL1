#include "Action.h"
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;


ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string _errorMsg) {
    errorMsg = _errorMsg;
    status = ActionStatus::ERROR;
    cout << "Error: " + errorMsg;
}

const string &BaseAction::getErrorMsg() const {
    return errorMsg;
}

SimulateStep::SimulateStep(const int _numOfSteps) : numOfSteps(_numOfSteps) {}

void SimulateStep::act(Simulation &simulation) {
    for (i = 0; i < numOfSteps; i++) {
        vector<Plan>* pl = simulation.getPlanList();// not sure about the &
        vector<Plan>::iterator itr;
        for (itr = (pl -> begin()); itr != (pl -> end());) {
            if (itr -> getPlanStatus() == PlanStatus::AVALIABLE) {
                (itr -> getUnderConstruction()).emplace_back((itr -> getSelecetionPolicy()).selectFacility(*simulation.getFacilityOptions()));
                itr -> setPlanStatus();
            } else {
                vector<Facility*>& ucl = itr -> getUnderConstruction();// not sure about the &
                vector<Facility*>::iterator itr_ucl;
                for (itr_ucl = ucl.begin(); itr_ucl != ucl.end(); itr_ucl++) {
                    itr_ucl -> step();
                    if (itr_ucl -> getStatus() == FacilityStatus::OPERATIONAL) {
                        itr -> addFacility(itr_ucl);
                    }
                }
                itr -> setPlanStatus();
                itr++;
            }
        }
    }
    complete();
}

const string SimulateStep::toString() const {
    return "The number of steps is: " + numOfSteps;
}

SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(*this);
}

AddPlan::AddPlan(const string &_settlementName, const string &_selectionPolicy) : settlementName(_settlementName), selectionPolicy(_selectionPolicy) {}

void AddPlan::act(Simulation &simulation) {
    bool errorCatch = true;
    for (const string sp : {"nve", "bal", "eco", "env"}) {
        if (sp == selectionPolicy) {
            errorCatch == false;
            break;
        }
    }

    if (!simulation.isSettlementExists(settlementName) or errorCatch) {
        error("Cannot create this plan");}//check what to do after error
    
    if (SelectionPolicy == "nve") {
        NaiveSelection::NaiveSelection* _sp = new NaiveSelection();
    } else if (SelectionPolicy == "bal") {
        BalancedSelection::BalancedSelection* _sp = new BalancedSelection(0, 0, 0);
    } else if (SelectionPolicy == "eco") {
        EconomySelection::EconomySelection* _sp = new EconomySelection();
    } else if (SelectionPolicy == "env") {
        SustainabilitySelection::SustainabilitySelection _sp = new SustainabilitySelection();
    }
    simulation.AddPlan(simulation.getSettlement(settlementName), _sp);
}