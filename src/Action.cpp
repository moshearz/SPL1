#include "Action.h"
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Empty Constructor 

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string _errorMsg) {
    errorMsg = _errorMsg;
    status = ActionStatus::ERROR;
    cout << "Error: " << errorMsg;
}

const string& BaseAction::getErrorMsg() const {
    return errorMsg;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


SimulateStep::SimulateStep(const int _numOfSteps) : numOfSteps(_numOfSteps) {}

void SimulateStep::act(Simulation &simulation) override {
    for (i = 0; i < numOfSteps; i++) {simulation.step();}
    complete();
}

const string SimulateStep::toString() const override {
    return "step " << numOfSteps << (status == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
}

SimulateStep* SimulateStep::clone() const override {
    return new SimulateStep(*this);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


AddPlan::AddPlan(const string& _settlementName, const string& _selectionPolicy) 
: settlementName(_settlementName), selectionPolicy(_selectionPolicy) {}

void AddPlan::act(Simulation &simulation) override {
    bool errorCatch = true;
    for (const string sp : {"nve", "bal", "eco", "env"}) {
        if (sp == selectionPolicy) {
            errorCatch = false;
            break;
        }
    }

    if (!simulation.isSettlementExists(settlementName) or errorCatch) {
        error("Cannot create this plan");}//check what to do after error
    else {
        simulation.AddPlan(simulation.getSettlement(settlementName), Plan::createSelectionPolicy(selectionPolicy));
        complete();
    }
}

const string AddPlan::toString() const override {
    return "plan " << settlementName << " " << selectionPolicy.toString() << (status == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
}

AddPlan* AddPlan::clone() const override {
    return new AddPlan(*this);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


AddSettlement::AddSettlement(const string& _settlementName, SettlementType _settlementType) 
: settlementName(_settlementName), settlementType(_settlementType) {}

void AddSettlement::act(Simulation &simulation) override {
    Settlement* tempSettlement = new Settlement(settlementName, settlementType);
    if (simulation.AddSettlement(tempSettlement)) {complete();}
    else {
        delete tempSettlement;
        error("Settlement already exists");
    }
}

AddSettlement* AddSettlement::clone() const override {
    return new AddSettlement(*this);
}

const string AddSettlement::toString() const override {
    return "settlement " << settlementName
    << (settlementType == SettlementType::VILLAGE ? " 0" : (settlementType == SettlementType::CITY ? " 1" : " 2"))
    << (status == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


AddFacility::AddFacility(const string& _facilityName, const FacilityCategory _facilityCategory, const int _price, const int _lifeQualityScore, const int _economyScore, const int _environmentScore)
            : facilityName(_facilityName), facilityCategory(_facilityCategory), price(_price), lifeQualityScore(_lifeQualityScore), economyScore(_economyScore), environmentScore(_environmentScore) {}

void AddFacility::act(Simulation &simulation) override {
    Facility tempFacility = FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore)
    if (simulation.addFacility(tempFacility)) {
        complete();
    } else {
        error("Facility already exists");
    }
}

AddFacility* AddFacility::clone() const override {
    return new AddFacility(*this);
}

const string AddFacility::toString() const override {
    return "facility " << facilityName << (facilityCategory == FacilityCategory::LIFE_QUALITY ? " 0 " : (facilityCategory == FacilityCategory::ECONOMY ? " 1 " : " 2 "))
    << price << " " << lifeQualityScore << " " << economyScore << " " << environmentScore
    << (status == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


PrintPlanStatus::PrintPlanStatus(int _planId) : planId(_planId) {}

void PrintPlanStatus::act(Simulation &simulation) override {
    if (simulation.isPlanExists(planId)) {
        simulation.getPlan(PlanId).printStatus();
        complete();
    } else {error("Plan doesn't exist");}
}

PrintPlanStatus* PrintPlanStatus::clone() const override {
    return new PrintPlanStatus(*this);
}

const string PrintPlanStatus::toString() const override {
    return "planStatus " << planId << (status == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//rule of 5 in changePlanPolicy

ChangePlanPolicy::ChangePlanPolicy(const int _planId, const string& _newPolicy) : planId(_planId), newPolicy(_newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation) override {
    bool errorCatch = true;
    for (const string sp : {"nve", "bal", "eco", "env"}) {
        if (sp == newPolicy) {
            errorCatch == false;
            break;
        }
    }
    if (!simulation.isPlanExists(planId) or errorCatch or newPolicy == simulation.getPlan(planId).toString()) {
        error("Cannot change selection policy");
    } else {
        simulation.getPlan(planId).setSelectionPolicy(Plan::createSelectionPolicy(newPolicy));
        complete();
    }
}

ChangePlanPolicy* ChangePlanPolicy::clone() const override {
    return new ChangePlanPolicy(*this);
}

const string ChangePlanPolicy::toString() const override {
    return "changePolicy " << planId << " " << newPolicy << (status == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Simulation &simulation) override {
    simulation.printActionsLog();
    complete();
}

PrintActionsLog* PrintActionsLog::clone() const override {
    return new PrintActionsLog(*this);
}

const string PrintActionsLog::toString() const override {
    return "log" << (status == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Close::Close() {}

void Close::act(Simulation &simulation) override {
    simulation.close();
    complete();
}

BackupSimulation* Close::clone() const override {
    return new Close(*this);
}

const string Close::toString() const override {
    return "close" << (status == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BackupSimulation::BackupSimulation() {}

extern Simulation* backup;

void BackupSimulation::act(Simulation &simulation) override {
    if (backup == nullptr) {backup = new Simulation(simulation);}
    else {(*backup) = simulation;}
    complete();
}

BackupSimulation* BackupSimulation::clone() const override {
    return new BackupSimulation(*this);
}

const string BackupSimulation::toString() const override {
    return "backup" << (status == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


RestoreSimulation::RestoreSimulation() {}

void RestoreSimulation::act(Simulation &simulation) override {
    if (backup == nullptr) {error("No backup available");}
    else {
        simulation = *backup;
        complete();
    }
}

RestoreSimulation* RestoreSimulation::clone() const override {
    return new RestoreSimulation(*this);
}

const string RestoreSimulation::toString() const override {
    return "restore" << (status == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
}