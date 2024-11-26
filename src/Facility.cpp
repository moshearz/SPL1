#include "Facility.h"

FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score) {}

const string &FacilityType::getName() const {
    return name;
}

int &FacilityType::getCost() const {
    return price;
}

int &FacilityType::getLifeQualityScore() const {
    return lifeQuality_score;
}

int &FacilityType::getEnvironmentScore() const {
    return environment_score;
}

int &FacilityType::getEconomyScore() const {
    return economy_score;
}

FacilityCategory &FacilityType::getCategory() const {
    return category;
}

Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score) {}
Facility::Facility(const FacilityType &type, const string &settlementName) {}

const string &Facility::getSettlementName() const {
    return settlementName;
}

const int &Facility::getTimeLeft() const {
    return timeLeft;
}

FacilityStatus &Facility::step() {
    if(timeLeft > 0) {
        timeLeft--;
    } else {
        status = FacilityStatus::OPERATIONAL;
    }
}

void &Facility::setStatus(FacilityStatus new_status) {
    status = new_status;
}

const &Facility::getStatus() const {
    return status;
}

const string &Facility::toString() const {
    return 
}