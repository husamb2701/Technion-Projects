#pragma once
#include "HunterNode.h"
#include "wet2util.h"

class Hunter {
private:
    int hunterId;
    NenAbility ability;
    int aura;
    int fightsMade;
    HunterNode* uf = nullptr;

public:
    Hunter(int hunterId, NenAbility ability, int aura, int fightsMade)
        : hunterId(hunterId), ability(ability), aura(aura), fightsMade(fightsMade), uf(nullptr) {};

    ~Hunter() = default;

    int getHunterId() const { return hunterId; }
    int getAura() const { return aura; }
    int getFightsMade() const { return fightsMade; }
    NenAbility getAbility() const { return ability; }

    void addFights(int num) {
        this->fightsMade += num;
    }

    HunterNode* getUFnode() const { return uf; }

    void setUFnode(HunterNode* uf_node) {
        this->uf = uf_node;
    }
};