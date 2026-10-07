#pragma once
#include <memory>
#include <string>

#define BASE_COINS 10
#define ARCHER_COINS 20
#define BASE_MAXHP 100
#define WARRIOR_MAXHP 150
#define BASE_LEVEL 1
#define Base_FORCE 5
#define POTION_COST 5
#define POTION_EFFECT 10
#define RISK_TAKER_CONDITION 50
#define WARRIOR_FACTOR 2
#define MAX_LEVEL 10

class Job;
class Character;
class Encounter;
class SolarEclipse;
class PotionMerchant;
using namespace std;

class Player {
public:
    Player(string name, unique_ptr<Job> job, unique_ptr<Character> character);
    Player(const Player& other);
    Player& operator=(const Player& other);
    string getDescription() const;
    string getName() const;
    int getLevel() const;
    void setLevel(int level);
    int getForce() const;
    void setForce(int force);
    int getHealthPoints() const;
    void setHealthPoints(int HealtPoints);
    int getCoins() const;
    void setCoins(int Coins);
    int getMaxHealthPoints() const;
    void setMaxHealthPoints(int MaxHealthPoints);
    bool isDead() const;
    string handleEvent(Encounter& monster);
    string handleEvent(const SolarEclipse& solar);
    string handleEvent(PotionMerchant& merchant);

private:
    string name;
    int level;
    int force;
    int currentHp;
    int maxHp;
    int coins;
    unique_ptr<Job> job;
    unique_ptr<Character> character;
};

class Job {
public:
    virtual ~Job() = default;
    virtual bool isRanged() const;
    virtual int getCombatPower(const Player& player) const;
    virtual int getInitialCoins() const;
    virtual string getJobName() const = 0;
    virtual int getMaxHP() const;
    virtual string handleEvent(Encounter& monster, Player& player);
    virtual string handleEvent(const SolarEclipse&, Player& player);
};

class Warrior : public Job {
public:
    ~Warrior() override = default;
    int getCombatPower(const Player& player) const override;
    string getJobName() const override;
    int getMaxHP() const override;
};

class Archer : public Job {
public:
    ~Archer() override = default;
    string getJobName() const override;
    int getInitialCoins() const override;
    bool isRanged() const override;
};

class Magician : public Job {
public:
    ~Magician() override = default;
    string getJobName() const override;
    bool isRanged() const override;
    string handleEvent(const SolarEclipse&, Player& player) override;
};

class Character {
public:
    virtual ~Character() = default;
    virtual string getCharacter() const = 0;
    virtual string handleEvent(PotionMerchant&, Player& player) = 0;
};

class Responsible : public Character {
public:
    ~Responsible() override = default;
    string getCharacter() const override;
    string handleEvent(PotionMerchant&, Player& player) override;
};

class RiskTaking : public Character {
public:
    ~RiskTaking() override = default;
    string getCharacter() const override;
    string handleEvent(PotionMerchant&, Player& player) override;
};

class JobFactory {
public:
    static unique_ptr<Job> createJob(const string& jobName);
};

class CharacterFactory {
public:
    static unique_ptr<Character> createCharacter(const string& jobName);
};

class PlayerFactory {
public:
    static Player createPlayer(const string& Name, unique_ptr<Job> job,
        unique_ptr<Character> character);
};
