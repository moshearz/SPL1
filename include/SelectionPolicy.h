#pragma once
#include <vector>
#include "Facility.h"
using std::vector;

class SelectionPolicy {
    public:
        virtual const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) = 0;
        virtual const string toString() const = 0;
        virtual SelectionPolicy* clone() const = 0;
        virtual ~SelectionPolicy() = default;

};

class NaiveSelection: public SelectionPolicy {
    public:
        NaiveSelection();
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        NaiveSelection *clone() const override;
        ~NaiveSelection() override = default;

        NaiveSelection(const NaiveSelection&) = default;
        NaiveSelection& operator=(const NaiveSelection&) = default;
        NaiveSelection(NaiveSelection&&) noexcept = default;
        NaiveSelection& operator=(NaiveSelection&&) noexcept = default;

    private:
        int lastSelectedIndex;
};

class BalancedSelection: public SelectionPolicy {
    public:
        BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore);
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        BalancedSelection *clone() const override;
        ~BalancedSelection() override = default;

        BalancedSelection(const BalancedSelection&) = default;
        BalancedSelection& operator=(const BalancedSelection&) = default;
        BalancedSelection(BalancedSelection&&) noexcept = default;
        BalancedSelection& operator=(BalancedSelection&&) noexcept = default;

    private:
        int LifeQualityScore;
        int EconomyScore;
        int EnvironmentScore;
};

class EconomySelection: public SelectionPolicy {
    public:
        EconomySelection();
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        EconomySelection *clone() const override;
        ~EconomySelection() override = default;

        EconomySelection(const EconomySelection&) = default;
        EconomySelection& operator=(const EconomySelection&) = default;
        EconomySelection(EconomySelection&&) noexcept = default;
        EconomySelection& operator=(EconomySelection&&) noexcept = default;

    private:
        int lastSelectedIndex;

};

class SustainabilitySelection: public SelectionPolicy {
    public:
        SustainabilitySelection();
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        SustainabilitySelection *clone() const override;
        ~SustainabilitySelection() override = default;

        SustainabilitySelection(const SustainabilitySelection&) = default;
        SustainabilitySelection& operator=(const SustainabilitySelection&) = default;
        SustainabilitySelection(SustainabilitySelection&&) noexcept = default;
        SustainabilitySelection& operator=(SustainabilitySelection&&) noexcept = default;
        
    private:
        int lastSelectedIndex;
};