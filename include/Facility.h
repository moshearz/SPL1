#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
using std::string;
using std::vector;

enum class FacilityStatus {
    UNDER_CONSTRUCTIONS,
    OPERATIONAL,
};

enum class FacilityCategory {
    LIFE_QUALITY,
    ECONOMY,
    ENVIRONMENT,
};


class FacilityType {
    public:
        FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score);
        const string &getName() const;
        int getCost() const;
        int getLifeQualityScore() const;
        int getEnvironmentScore() const;
        int getEconomyScore() const;
        FacilityCategory getCategory() const;

        FacilityType *clone() const;
        ~FacilityType() = default;
        FacilityType(const FacilityType &other) = default;
        FacilityType(FacilityType &&other) = default;
        FacilityType& operator=(const FacilityType& other) = delete;
        FacilityType& operator=(FacilityType&& other) = delete;

    protected:
        const string name;
        const FacilityCategory category;
        const int price;
        const int lifeQuality_score;
        const int economy_score;
        const int environment_score;
};



class Facility: public FacilityType {

    public:
        Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score);
        Facility(const FacilityType &type, const string &settlementName);
        const string &getSettlementName() const;
        const int getTimeLeft() const;
        FacilityStatus step();
        void setStatus(FacilityStatus status);
        const FacilityStatus& getStatus() const;
        const string toString() const;

        ~Facility() = default;
        Facility(const Facility &other) = default;
        Facility(Facility &&other) = default;
        Facility& operator=(const Facility& other) = delete;
        Facility& operator=(Facility&& other) = delete;

    private:
        const string settlementName;
        FacilityStatus status;
        int timeLeft;
};