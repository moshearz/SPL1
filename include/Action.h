#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "Simulation.h"
enum class SettlementType;
enum class FacilityCategory;

enum class ActionStatus{
    COMPLETED, ERROR
};

class BaseAction{
    public:
        BaseAction();
        ActionStatus getStatus() const;
        virtual void act(Simulation& simulation)=0;
        virtual const string toString() const=0;
        virtual BaseAction* clone() const = 0;
        virtual ~BaseAction() = default;

    protected:
        void complete();
        void error(string errorMsg);
        const string &getErrorMsg() const;

    private:
        string errorMsg;
        ActionStatus status;
};

class SimulateStep : public BaseAction {

    public:
        SimulateStep(const int numOfSteps);
        void act(Simulation &simulation) override;
        const string toString() const override;
        SimulateStep *clone() const override;

        ~SimulateStep() = default;
        SimulateStep(const SimulateStep& other) = default;
        SimulateStep(SimulateStep&& other) = default;
        SimulateStep& operator=(const SimulateStep& other) = delete;
        SimulateStep& operator=(SimulateStep&& other) = delete;

    private:
        const int numOfSteps;
};

class AddPlan : public BaseAction {
    public:
        AddPlan(const string &settlementName, const string &selectionPolicy);
        void act(Simulation &simulation) override;
        const string toString() const override;
        AddPlan *clone() const override;

        ~AddPlan() = default;
        AddPlan(const AddPlan& other) = default;
        AddPlan(AddPlan&& other) = default;
        AddPlan& operator=(const AddPlan& other) = delete;
        AddPlan& operator=(AddPlan&& other) = delete;

    private:
        const string settlementName;
        const string selectionPolicy;
};


class AddSettlement : public BaseAction {
    public:
        AddSettlement(const string &settlementName,SettlementType settlementType);
        void act(Simulation &simulation) override;
        AddSettlement *clone() const override;
        const string toString() const override;

        ~AddSettlement() = default;
        AddSettlement(const AddSettlement& other) = default;
        AddSettlement(AddSettlement&& other) = default;
        AddSettlement& operator=(const AddSettlement& other) = delete;
        AddSettlement& operator=(AddSettlement&& other) = delete;

    private:
        const string settlementName;
        const SettlementType settlementType;
};



class AddFacility : public BaseAction {
    public:
        AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore);
        void act(Simulation &simulation) override;
        AddFacility *clone() const override;
        const string toString() const override;

        ~AddFacility() = default;
        AddFacility(const AddFacility& other) = default;
        AddFacility(AddFacility&& other) = default;
        AddFacility& operator=(const AddFacility& other) = delete;
        AddFacility& operator=(AddFacility&& other) = delete;

    private:
        const string facilityName;
        const FacilityCategory facilityCategory;
        const int price;
        const int lifeQualityScore;
        const int economyScore;
        const int environmentScore;

};

class PrintPlanStatus: public BaseAction {
    public:
        PrintPlanStatus(int planId);
        void act(Simulation &simulation) override;
        PrintPlanStatus *clone() const override;
        const string toString() const override;

        ~PrintPlanStatus() = default;
        PrintPlanStatus(const PrintPlanStatus& other) = default;
        PrintPlanStatus(PrintPlanStatus&& other) = default;
        PrintPlanStatus& operator=(const PrintPlanStatus& other) = delete;
        PrintPlanStatus& operator=(PrintPlanStatus&& other) = delete;

    private:
        const int planId;
};


class ChangePlanPolicy : public BaseAction {
    public:
        ChangePlanPolicy(const int planId, const string &newPolicy);
        void act(Simulation &simulation) override;
        ChangePlanPolicy *clone() const override;
        const string toString() const override;

        ~ChangePlanPolicy() = default;
        ChangePlanPolicy(const ChangePlanPolicy& other) = default;
        ChangePlanPolicy(ChangePlanPolicy&& other) = default;
        ChangePlanPolicy& operator=(const ChangePlanPolicy& other) = delete;
        ChangePlanPolicy& operator=(ChangePlanPolicy&& other) = delete;

    private:
        const int planId;
        const string newPolicy;
};


class PrintActionsLog : public BaseAction {
    public:
        PrintActionsLog();
        void act(Simulation &simulation) override;
        PrintActionsLog *clone() const override;
        const string toString() const override;

    private:
};

class Close : public BaseAction {
    public:
        Close();
        void act(Simulation &simulation) override;
        Close *clone() const override;
        const string toString() const override;

    private:
};

class BackupSimulation : public BaseAction {
    public:
        BackupSimulation();
        void act(Simulation &simulation) override;
        BackupSimulation *clone() const override;
        const string toString() const override;

    private:
};


class RestoreSimulation : public BaseAction {
    public:
        RestoreSimulation();
        void act(Simulation &simulation) override;
        RestoreSimulation *clone() const override;
        const string toString() const override;

    private:
};