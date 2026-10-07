#include "Event.h"
#include <sstream>

using namespace std;

string Encounter::getDescription() const {
    return getEventName()
        + " (power " + to_string(getCombatPower())
        + ", loot " + to_string(getLoot())
        + ", damage " + to_string(getDamage())
        + ")";
}

string SpacialEvent::getDescription() const {
    return getEventName();
}

string Encounter::playEvent(Player &player) {
    return player.handleEvent(*this);
}

string Balrog::playEvent(Player &player) {
    string save = player.handleEvent(*this);
    setCombatPower(getCombatPower() + 2);
    return save;
}

string Pack::playEvent(Player &player) {
    return player.handleEvent(*this);
}

int Pack::getCombatPower() const {
    int totalCombatPower = 0;
    for (const unique_ptr<Encounter>& encounter : monsters) {
        if (encounter) totalCombatPower += encounter->getCombatPower();
    }
    return totalCombatPower;
}

int Pack::getLoot() const {
    int total_loot = 0;
    for (const unique_ptr<Encounter>& encounter : monsters) {
        if (encounter) total_loot += encounter->getLoot();
    }
    return total_loot;
}

int Pack::getDamage() const {
    int total_damage = 0;
    for (const unique_ptr<Encounter>& encounter : monsters) {
        if (encounter) total_damage += encounter->getDamage();
    }
    return total_damage;
}

int Pack::getMonsterCount() const {
    return static_cast<int>(monsters.size());
}

int Pack::checkBalrogs() const {
    int total_Balrogs = 0;
    for (const unique_ptr<Encounter>& encounter : monsters) {
        if (encounter && encounter->getEventName() == "Balrog")
            total_Balrogs++;
    }
    return total_Balrogs;
}

// תיקון: השתמשתי ב-stringstream לטיפול בטוח ב-Parsing למניעת שגיאות של תווים כמו \r או רווחים
unique_ptr<Event> EventFactory::createEvent(const string& eventLine) {
    if (eventLine.empty()) return nullptr;
    stringstream ss(eventLine);
    string eventName;
    ss >> eventName;

    if (eventName == "Snail") return make_unique<Snail>();
    if (eventName == "Slime") return make_unique<Slime>();
    if (eventName == "Balrog") return make_unique<Balrog>();
    if (eventName == "SolarEclipse") return make_unique<SolarEclipse>();
    if (eventName == "PotionsMerchant") return make_unique<PotionMerchant>();

    if (eventName == "Pack") {
        int count;
        ss >> count;
        if (count < 2) throw invalid_argument("Invalid Events File");

        vector<unique_ptr<Encounter>> monsters;
        for (int i = 0; i < count; ++i) {
            string monsterType;
            ss >> monsterType;
            if (monsterType == "Snail") monsters.push_back(make_unique<Snail>());
            else if (monsterType == "Slime") monsters.push_back(make_unique<Slime>());
            else if (monsterType == "Balrog") monsters.push_back(make_unique<Balrog>());
            else throw invalid_argument("Invalid Events File");
        }
        return make_unique<Pack>(move(monsters));
    }
    throw invalid_argument("Invalid Events File");
}
