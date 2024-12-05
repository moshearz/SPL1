#include "Action.h"
#include <iostream>
#include <sstream>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Empty Constructor 
BaseAction::BaseAction() : status(ActionStatus::COMPLETED), errorMsg("") {}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string _errorMsg) {
    errorMsg = _errorMsg;
    status = ActionStatus::ERROR;
    std::cout << "Error: " << errorMsg << "\n";
}

const string& BaseAction::getErrorMsg() const {
    return errorMsg;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


SimulateStep::SimulateStep(const int _numOfSteps) : numOfSteps(_numOfSteps) {}

void SimulateStep::act(Simulation &simulation) {
    for (int i = 0; i < numOfSteps; i++) {simulation.step();}
    complete();
}

const string SimulateStep::toString() const {
    std::ostringstream oss;
    oss << "step " << numOfSteps << (this -> getStatus() == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
    return oss.str();
}

SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(*this);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


AddPlan::AddPlan(const string& _settlementName, const string& _selectionPolicy) 
: settlementName(_settlementName), selectionPolicy(_selectionPolicy) {}

void AddPlan::act(Simulation &simulation) {
    bool errorCatch = true;
    for (const string sp : {"nve", "bal", "eco", "env"}) {
        if (sp == selectionPolicy) {
            errorCatch  = false;
            break;
        }
    }

    if (!simulation.isSettlementExists(settlementName) or errorCatch) {
        error("Cannot create this plan");}//check what to do after error
    else {
        simulation.addPlan(simulation.getSettlement(settlementName), simulation.createSelectionPolicy(selectionPolicy, 0, 0, 0));
        complete();
    }
}

const string AddPlan::toString() const {
    std::ostringstream oss;
    oss << "plan " << settlementName << " " << selectionPolicy << (this -> getStatus() == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
    return oss.str();
}

AddPlan* AddPlan::clone() const {
    return new AddPlan(*this);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


AddSettlement::AddSettlement(const string& _settlementName, SettlementType _settlementType) 
: settlementName(_settlementName), settlementType(_settlementType) {}

void AddSettlement::act(Simulation &simulation) {
    Settlement* tempSettlement = new Settlement(settlementName, settlementType);
    if (simulation.addSettlement(tempSettlement)) {complete();}
    else {
        delete tempSettlement;
        error("Settlement already exists");
    }
}

AddSettlement* AddSettlement::clone() const {
    return new AddSettlement(*this);
}

const string AddSettlement::toString() const {
    std::ostringstream oss;
    oss << "settlement " << settlementName
    << (settlementType == SettlementType::VILLAGE ? " 0" : (settlementType == SettlementType::CITY ? " 1" : " 2"))
    << (this -> getStatus() == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
    return oss.str();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


AddFacility::AddFacility(const string& _facilityName, const FacilityCategory _facilityCategory, const int _price, const int _lifeQualityScore, const int _economyScore, const int _environmentScore)
            : facilityName(_facilityName), facilityCategory(_facilityCategory), price(_price), lifeQualityScore(_lifeQualityScore), economyScore(_economyScore), environmentScore(_environmentScore) {}

void AddFacility::act(Simulation &simulation) {
    FacilityType temp_f(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    if (simulation.addFacility(std::move(temp_f))) {complete();} 
    else {error("Facility already exists");}
}

AddFacility* AddFacility::clone() const {
    return new AddFacility(*this);
}

const string AddFacility::toString() const {
    std::ostringstream oss;
    oss << "facility " << facilityName << (facilityCategory == FacilityCategory::LIFE_QUALITY ? " 0 " : (facilityCategory == FacilityCategory::ECONOMY ? " 1 " : " 2 "))
    << price << " " << lifeQualityScore << " " << economyScore << " " << environmentScore
    << (this -> getStatus() == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
    return oss.str();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


PrintPlanStatus::PrintPlanStatus(int _planId) : planId(_planId) {}

void PrintPlanStatus::act(Simulation &simulation) {
    if (simulation.isPlanExists(planId)) {
        simulation.getPlan(planId).printStatus();
        complete();
    } else {error("Plan doesn't exist");}
}

PrintPlanStatus* PrintPlanStatus::clone() const {
    return new PrintPlanStatus(*this);
}

const string PrintPlanStatus::toString() const {
    std::ostringstream oss;
    oss << "planStatus " << planId << (this -> getStatus() == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
    return oss.str();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//rule of 5 in changePlanPolicy

ChangePlanPolicy::ChangePlanPolicy(const int _planId, const string& _newPolicy) : planId(_planId), newPolicy(_newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation) {
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
        Plan plan = simulation.getPlan(planId);
        plan.setSelectionPolicy(simulation.createSelectionPolicy(newPolicy, plan.getlifeQualityScore(), plan.getEconomyScore(), plan.getEnvironmentScore()));
        complete();
    }
}

ChangePlanPolicy* ChangePlanPolicy::clone() const {
    return new ChangePlanPolicy(*this);
}

const string ChangePlanPolicy::toString() const {
    std::ostringstream oss;
    oss << "changePolicy " << planId << " " << newPolicy << (this -> getStatus() == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
    return oss.str();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Simulation &simulation) {
    simulation.printActionsLog();
    complete();
}

PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

const string PrintActionsLog::toString() const {
    std::ostringstream oss;
    oss << "log" << (this -> getStatus() == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
    return oss.str();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Close::Close() {}

void Close::act(Simulation &simulation) {
    simulation.close();
    complete();
}

Close* Close::clone() const {
    return new Close(*this);
}

const string Close::toString() const {
    std::ostringstream oss;
    oss << "close" << (this -> getStatus() == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
    return oss.str();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BackupSimulation::BackupSimulation() {}

extern Simulation* backup;

void BackupSimulation::act(Simulation &simulation) {
    delete backup;
    backup = new Simulation(simulation);
    complete();
}

BackupSimulation* BackupSimulation::clone() const {
    return new BackupSimulation(*this);
}

const string BackupSimulation::toString() const {
    std::ostringstream oss;
    oss << "backup" << (this -> getStatus() == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
    return oss.str();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


RestoreSimulation::RestoreSimulation() {}

void RestoreSimulation::act(Simulation &simulation) {
    if (backup == nullptr) {error("No backup available");}
    else {
        simulation.~Simulation();
        new (&simulation) Simulation(*backup);
        complete();
    }
}

RestoreSimulation* RestoreSimulation::clone() const {
    return new RestoreSimulation(*this);
}

const string RestoreSimulation::toString() const {
    std::ostringstream oss;
    oss << "restore" << (this -> getStatus() == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR");
    return oss.str();
}