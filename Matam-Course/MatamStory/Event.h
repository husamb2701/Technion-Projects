#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "Player.h"

using namespace std;

class Event {
public:
    virtual ~Event() = default;
    virtual string getDescription() const = 0;
    virtual string playEvent(Player& player) = 0;
    virtual string getEventName() const = 0;
};

class Encounter : public Event {
    int combatPower = 0;
    int Loot = 0;
    int Damage = 0;
public:
    ~Encounter() override = default;
    explicit Encounter(int combatPower = 0, int loot = 0, int damage = 0)
        : combatPower(combatPower), Loot(loot), Damage(damage)
    {}

    string getDescription() const override;
    string getEventName() const override = 0;
    virtual int getCombatPower() const { return combatPower; }
    virtual void setCombatPower(int combat_power) { combatPower = combat_power; }
    virtual int getLoot() const { return Loot; }
    virtual void setLoot(int loot) { Loot = loot; }
    virtual int getDamage() const { return Damage; }
    virtual void setDamage(int damage) { Damage = damage; }

    string playEvent(Player& player) override;
};

class Snail : public Encounter {
public:
    Snail() : Encounter(5, 2, 10) {}
    string getEventName() const override { return "Snail"; }
};

class Slime : public Encounter {
public:
    Slime() : Encounter(12, 5, 25) {}
    string getEventName() const override { return "Slime"; }
};

class Balrog : public Encounter {
public:
    Balrog() : Encounter(15, 100, 9001) {}
    string getEventName() const override { return "Balrog"; }
    string playEvent(Player& player) override;
};

class Pack : public Encounter {
    vector<unique_ptr<Encounter>> monsters;
public:
    explicit Pack(vector<unique_ptr<Encounter>> loadMonsters)
        : Encounter(0, 0, 0),
          monsters(move(loadMonsters))
    {}
    string getEventName() const override {
        return "Pack of " + to_string(getMonsterCount()) + " members";
    }
    int getCombatPower() const override;
    int getLoot() const override;
    int getDamage() const override;
    int getMonsterCount() const;
    int checkBalrogs() const;
    string playEvent(Player& player) override;
};

class SpacialEvent : public Event {
    string getDescription() const override;
    string getEventName() const override = 0;
};

class SolarEclipse : public SpacialEvent {
    string getEventName() const override { return "SolarEclipse"; }
    string playEvent(Player& player) override {
        return player.handleEvent(*this);
    }
};

class PotionMerchant : public SpacialEvent {
    string getEventName() const override { return "PotionsMerchant"; }
    string playEvent(Player& player) override {
        return player.handleEvent(*this);
    }
};

class EventFactory {
public:
    static unique_ptr<Event> createEvent(const string& eventName);
};
