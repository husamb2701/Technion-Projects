#include "Player.h"
#include <stdexcept>
#include <string>
#include <utility>
#include "../Utilities.h"

// ===== Player methods =====

Player::Player(string name, unique_ptr<Job> job, unique_ptr<Character> character) :
    name(std::move(name)),
    level(BASE_LEVEL),
    force(Base_FORCE),
    currentHp(job->getMaxHP()),
    maxHp(job->getMaxHP()),
    coins(job->getInitialCoins()),
    job(std::move(job)),
    character(std::move(character))
{}

Player::Player(const Player& other) {
    this->name = other.name;
    this->level = other.level;
    this->force = other.force;
    this->currentHp = other.currentHp;
    this->maxHp = other.maxHp;
    this->coins = other.coins;
    this->job = JobFactory::createJob(other.job->getJobName());
    this->character = CharacterFactory::createCharacter(other.character->getCharacter());
}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        this->name = other.name;
        this->level = other.level;
        this->force = other.force;
        this->currentHp = other.currentHp;
        this->maxHp = other.maxHp;
        this->coins = other.coins;
        this->job = JobFactory::createJob(other.job->getJobName());
        this->character = CharacterFactory::createCharacter(other.character->getCharacter());
    }
    return *this;
}

string Player::getDescription() const {
    return getName() + ", " + job->getJobName() + " with " +
           character->getCharacter() + " character (level " +
           to_string(getLevel()) + ", force " +
           to_string(getForce()) + ")";
}

string Player::getName() const {
    return this->name;
}

int Player::getLevel() const {
    return this->level;
}

void Player::setLevel(int level) {
    this->level = level;
}

int Player::getForce() const {
    return this->force;
}

void Player::setForce(int force) {
    this->force = force;
}

int Player::getHealthPoints() const {
    return this->currentHp;
}

void Player::setHealthPoints(int healthPoints) {
    this->currentHp = healthPoints;
}

int Player::getCoins() const {
    return this->coins;
}

void Player::setCoins(int coins) {
    this->coins = coins;
}

int Player::getMaxHealthPoints() const {
    return this->maxHp;
}

void Player::setMaxHealthPoints(int maxHealthPoints) {
    this->maxHp = maxHealthPoints;
}

bool Player::isDead() const {
    return this->currentHp <= 0;
}

string Player::handleEvent(Encounter& monster) {
    return job->handleEvent(monster, *this);
}

string Player::handleEvent(const SolarEclipse& solar) {
    return job->handleEvent(solar, *this);
}

string Player::handleEvent(PotionMerchant& merchant) {
    return character->handleEvent(merchant, *this);
}

// ===== Job methods =====

bool Job::isRanged() const {
    return false;
}

int Job::getCombatPower(const Player& player) const {
    return player.getForce() + player.getLevel();
}

int Job::getInitialCoins() const {
    return BASE_COINS;
}

string Job::handleEvent(Encounter &monster, Player &player) {
    if (getCombatPower(player) < monster.getCombatPower()) {
        if (player.getHealthPoints() > monster.getCombatPower()) {
            if (player.getHealthPoints() - monster.getDamage() >= 0) {
                player.setHealthPoints(player.getHealthPoints() - monster.getDamage());
            } else {
                player.setHealthPoints(0);
            }
        } else {
            player.setHealthPoints(0);
        }
        return getEncounterLostMessage(player, monster.getDamage());
    } else {
        player.setLevel(player.getLevel() + 1);
        player.setCoins(player.getCoins() + monster.getLoot());
        if (!isRanged()) {
            player.setHealthPoints(player.getHealthPoints() - 10);
        }
        return getEncounterWonMessage(player, monster.getLoot());
    }
}

string Job::handleEvent(const SolarEclipse&, Player &player) {
    int effect = -1;
    int noEffect = 0;
    if (player.getForce() == noEffect) {
        return getSolarEclipseMessage(player, noEffect);
    }
    player.setForce(player.getForce() + effect);
    return getSolarEclipseMessage(player, effect);
}

int Job::getMaxHP() const {
    return BASE_MAXHP;
}

// ===== Warrior =====

int Warrior::getCombatPower(const Player& player) const {
    return player.getForce() * WARRIOR_FACTOR + player.getLevel();
}

string Warrior::getJobName() const {
    return "Warrior";
}

int Warrior::getMaxHP() const {
    return WARRIOR_MAXHP;
}

// ===== Archer =====

string Archer::getJobName() const {
    return "Archer";
}

int Archer::getInitialCoins() const {
    return ARCHER_COINS;
}

bool Archer::isRanged() const {
    return true;
}

// ===== Magician =====

string Magician::getJobName() const {
    return "Magician";
}

bool Magician::isRanged() const {
    return true;
}

string Magician::handleEvent(const SolarEclipse&, Player &player) {
    int effect = 1;
    player.setForce(player.getForce() + effect);
    return getSolarEclipseMessage(player, effect);
}

// ===== Responsible (Character) =====

string Responsible::getCharacter() const {
    return "Responsible";
}

string Responsible::handleEvent(PotionMerchant&, Player& player) {
    unsigned int count = 0;
    while (player.getHealthPoints() < player.getMaxHealthPoints() &&
           player.getCoins() >= POTION_COST) {
        player.setCoins(player.getCoins() - POTION_COST);
        if (player.getMaxHealthPoints() - player.getHealthPoints() >= POTION_EFFECT) {
            player.setHealthPoints(player.getHealthPoints() + POTION_EFFECT);
        } else {
            player.setHealthPoints(player.getMaxHealthPoints());
        }
        count++;
    }
    return getPotionsPurchaseMessage(player, count);
}

// ===== RiskTaking (Character) =====

string RiskTaking::getCharacter() const {
    return "RiskTaking";
}

string RiskTaking::handleEvent(PotionMerchant&, Player& player) {
    unsigned int count = 0;
    if (player.getHealthPoints() < RISK_TAKER_CONDITION &&
        player.getCoins() >= POTION_COST) {
        player.setCoins(player.getCoins() - POTION_COST);
        player.setHealthPoints(player.getHealthPoints() + POTION_EFFECT);
        count = 1;
    }
    return getPotionsPurchaseMessage(player, count);
}

unique_ptr<Job> JobFactory::createJob(const string& jobName) {
    if (jobName == "Warrior") return make_unique<Warrior>();
    if (jobName == "Archer") return make_unique<Archer>();
    if (jobName == "Magician") return make_unique<Magician>();
    throw invalid_argument("Invalid Players File");
}

unique_ptr<Character> CharacterFactory::createCharacter(const string& characterName) {
    if (characterName == "Responsible") return make_unique<Responsible>();
    if (characterName == "RiskTaking") return make_unique<RiskTaking>();
    throw invalid_argument("Invalid Players File");
}

Player PlayerFactory::createPlayer(const string& name, unique_ptr<Job> job,
        unique_ptr<Character> character) {
    if (name.length() > 15) {
        throw invalid_argument("Invalid Players File");
    }
    return {name, move(job), move(character)};
}
