#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

class Facility;

enum class SettlementType {
    VILLAGE,
    CITY,
    METROPOLIS,
};

class Settlement {
    public:
        Settlement(const string &name, SettlementType type);
        const string &getName() const;
        SettlementType getType() const;
        const string toString() const;

        ~Settlement() = default;
        Settlement(const Settlement& other) = default;
        Settlement(Settlement&& other) = default;
        Settlement& operator=(const Settlement& other) = delete;
        Settlement& operator=(Settlement&& other) = delete;
        

    private:
        const string name;
        SettlementType type;
};