#pragma once

#include "Squad.h"

#include "wet2util.h"

#include "memory"

class Squad;


class HunterNode {
private:
    int hunterId;
    NenAbility ability;
    int aura;
    int currentFights;

    HunterNode* parent;
    int rank;

    std::weak_ptr<Squad> squad;

    int deltaFights;
    NenAbility deltaNen;

public:
    HunterNode(int hunterId, const NenAbility& ability, int aura, int fightsMade)
        : hunterId(hunterId),
          ability(ability),
          aura(aura),
          currentFights(fightsMade),
          parent(nullptr),
          rank(0),
          squad(),
          deltaFights(0),
          deltaNen()
    {}
    ~HunterNode() = default;
    int getHunterId() const { return hunterId; }
    int getAura() const { return aura; }
    int getCurrentFights() const { return currentFights; }
    int getRank() const { return rank; }
    int getDeltaFights() const { return deltaFights; }
    std::weak_ptr<Squad> getSquad() const { return squad; }
    NenAbility& getAbility() { return ability; }
    NenAbility& getDeltaNen() { return deltaNen; }
    void setSquad(const std::weak_ptr<Squad>& squad_) { this->squad = squad_; }
    void setRank(int rank_) { rank = rank_; }
    void setDeltaFights(int deltaFights_) { deltaFights = deltaFights_; }
    void setAura(int aura_) { aura = aura_; }
    void setDeltaNen(const NenAbility& deltaNen_) { deltaNen = deltaNen_; }

    static HunterNode *find(HunterNode *node) {
        if (node == nullptr) {
            return nullptr;
        }
        if (node->parent == nullptr) {
            return node;
        } else {
            HunterNode *parent = node->parent;
            HunterNode *root = find(parent);
            if (parent != root) {
                //avoiding adding root to itself
                node->deltaFights += parent->deltaFights;
                node->deltaNen += parent->deltaNen;
            }
            node->parent = root;
            return root;
        }
    }


    static void unite(HunterNode *nodeA, HunterNode *nodeB, const std::shared_ptr<Squad> &forcing_squad) {
    if (nodeA == nullptr || nodeB == nullptr) return;
    HunterNode *rootA = find(nodeA);
    HunterNode *rootB = find(nodeB);
    if (rootA == rootB) return;
    int addA = 0;
    int addB = 0;
    NenAbility nenA;
    std::shared_ptr<Squad> squadA = rootA->squad.lock();
    if (squadA) {
        addA = squadA->getFightsOfSquad();
        nenA = squadA->getTotalNen();
    }
    std::shared_ptr<Squad> squadB = rootB->squad.lock();
    if (squadB) {
        addB = squadB->getFightsOfSquad();
    }
    if (rootA->rank > rootB->rank) {
        rootB->parent = rootA;
        rootB->deltaFights += (addB - addA) - rootA->deltaFights;
        rootB->deltaNen += (nenA - rootA->deltaNen);
        rootA->squad = forcing_squad;
        forcing_squad->setHunterNode(rootA);
    } else if (rootA->rank < rootB->rank) {
        rootA->parent = rootB;
        rootB->deltaFights += (addB - addA);
        rootA->deltaFights -= rootB->deltaFights;
        rootB->deltaNen += nenA;
        rootA->deltaNen -= rootB->deltaNen;
        rootB->squad = forcing_squad;
        forcing_squad->setHunterNode(rootB);
    } else {
        rootB->parent = rootA;
        rootB->deltaFights += (addB - addA) - rootA->deltaFights;
        rootB->deltaNen += (nenA - rootA->deltaNen);
        rootA->squad = forcing_squad;
        rootA->rank++;
        forcing_squad->setHunterNode(rootA);
    }
}
};