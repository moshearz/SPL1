#include "Facility.h"
#include <sstream> // For std::ostringstream

FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price,
                           const int lifeQuality_score, const int economy_score, const int environment_score)
    : name(name), category(category), price(price),
      lifeQuality_score(lifeQuality_score), economy_score(economy_score),
      environment_score(environment_score) {}

const string &FacilityType::getName() const {
    return name;
}

int FacilityType::getCost() const { // Return by value
    return price;
}

int FacilityType::getLifeQualityScore() const { // Return by value
    return lifeQuality_score;
}

int FacilityType::getEnvironmentScore() const { // Return by value
    return environment_score;
}

int FacilityType::getEconomyScore() const { // Return by value
    return economy_score;
}

FacilityCategory FacilityType::getCategory() const { // Return by value
    return category;
}

Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price,
                   const int lifeQuality_score, const int economy_score, const int environment_score)
    : FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score),
      settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price) {}

Facility::Facility(const FacilityType &type, const string &settlementName)
    : FacilityType(type), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(type.getCost()) {}

const string &Facility::getSettlementName() const {
    return settlementName;
}

int Facility::getTimeLeft() const { // Return by value
    return timeLeft;
}

FacilityStatus Facility::step() { // Return by value
    if (timeLeft > 0) {
        --timeLeft;
        if (timeLeft == 0) {
            status = FacilityStatus::OPERATIONAL;
        }
    }
    return status;
}

void Facility::setStatus(FacilityStatus new_status) { // Return void
    status = new_status;
}

const FacilityStatus &Facility::getStatus() const { // Return const reference
    return status;
}

string Facility::toString() const { // Return by value
    std::ostringstream oss;
    oss << "Facility: " << getName() << ", Settlement: " << settlementName
        << ", Status: " << (status == FacilityStatus::UNDER_CONSTRUCTIONS ? "Under Construction" : "Operational")
        << ", Time Left: " << timeLeft;
    return oss.str();
}
