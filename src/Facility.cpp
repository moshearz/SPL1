#include "Facility.h"
#include <sstream> // For std::ostringstream

FacilityType::FacilityType(const string& _name, const FacilityCategory _category, const int _price,
                           const int _lifeQuality_score, const int _economy_score, const int _environment_score)
    : name(_name), category(_category), price(_price),
      lifeQuality_score(_lifeQuality_score), economy_score(_economy_score),
      environment_score(_environment_score) {}

const string& FacilityType::getName() const {
    return name;
}

int FacilityType::getCost() const { 
    return price;
}

int FacilityType::getLifeQualityScore() const { 
    return lifeQuality_score;
}

int FacilityType::getEnvironmentScore() const { 
    return environment_score;
}

int FacilityType::getEconomyScore() const { 
    return economy_score;
}

FacilityCategory FacilityType::getCategory() const { 
    return category;
}

FacilityType::FacilityType(const FacilityType& other)
    : name(other.name), category(other.category), price(other.price),
      lifeQuality_score(other.lifeQuality_score), economy_score(other.economy_score),
      environment_score(other.environment_score) {}

FacilityType::FacilityType(FacilityType&& other) noexcept
    : name(std::move(other.name)), category(other.category), price(other.price),
      lifeQuality_score(other.lifeQuality_score), economy_score(other.economy_score),
      environment_score(other.environment_score) {}


FacilityType* FacilityType::clone() const {
    return new FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score);
}









Facility::Facility(const string& _name, const string& _settlementName, const FacilityCategory _category, const int _price,
    const int _lifeQuality_score, const int _economy_score, const int _environment_score)
    :FacilityType(_name, _category, _price, _lifeQuality_score, _economy_score, _environment_score),
    status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(_price) {}

Facility::Facility(const FacilityType& _type, const string& _settlementName) : settlementName(_settlementName), 
    status(FacilityStatus::UNDER_CONSTRUCTIONS), FacilityType(_type.getName(), _type.getCategory(), _type.getCost(), 
    _type.getLifeQualityScore(), _type.getEconomyScore(), _type.getEnvironmentScore()) {}

const string& Facility::getSettlementName() const {
    return settlementName;
}

const int Facility::getTimeLeft() const { 
    return timeLeft;
}

FacilityStatus Facility::step() { 
    if (timeLeft > 0) {
        --timeLeft;
        if (timeLeft == 0) {
            status = FacilityStatus::OPERATIONAL;
        }
    }
    return status;
}

void Facility::setStatus(FacilityStatus _status) { 
    status = _status;
}

const FacilityStatus& Facility::getStatus() const { 
    return status;
}

const string Facility::toString() const { 
    std::ostringstream oss;
    oss << "Facility: " << getName() << ", Settlement: " << settlementName
        << ", Status: " << (status == FacilityStatus::UNDER_CONSTRUCTIONS ? "Under Construction" : "Operational")
        << ", Time Left: " << timeLeft;
    return oss.str();
}
