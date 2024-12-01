#include "Action.h"
#include <iostream>
#include <vector>
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
    vector<Plan> pl = simulation.getPlanList();
    vector<Plan>::iterator itr;
    for (itr = pl.begin(); itr != pl.end();) {
        if (itr -> getPlanStatus() == 0) {
            itr -> addFacility((itr -> getSelecetionPolicy()).selectFacility(simulation.getFacilityOptions()));
        } else {
            vector<Facility*> ucl = itr -> getUnderConstruction();
            vector<Facility*>::iterator itr_ucl;
            for (itr_ucl = ucl.begin(); itr_ucl != ucl.end(); itr_ucl++) {
                itr_ucl -> step();
                if (itr_ucl -> getStatus() == FacilityStatus::OPERATIONAL) {
                    
                }
            }
            itr++;
        }
    }
}