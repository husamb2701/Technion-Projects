#pragma once
#include "wet2util.h"

class HunterNode;

class Squad {
private:
    int squadId;
    int exp;
    int totalAura;
    NenAbility totalNen;
    int fightsOfSquad;
    HunterNode* hunterNode;

public:
    explicit Squad(int squadId)
        : squadId(squadId),
          exp(0),
          totalAura(0),
          fightsOfSquad(0),
          hunterNode(nullptr)
    {}
    ~Squad() = default;

    int getSquadId() const { return squadId; }
    int getExp() const { return exp; }
    int getTotalAura() const { return totalAura; }
    const NenAbility& getTotalNen() const { return totalNen; }
    int getFightsOfSquad() const { return fightsOfSquad; }
    HunterNode* getHunterNode() const { return hunterNode; }

    void setSquadId(int squadId) { this->squadId = squadId; }
    void setExp(int exp) { this->exp = exp; }
    void setTotalAura(int aura) { this->totalAura = aura; }
    void setTotalNen(const NenAbility& nen) { this->totalNen = nen; }
    void setFightsOfSquad(int fights) { this->fightsOfSquad = fights; }
    void setHunterNode(HunterNode* hunter) { this->hunterNode = hunter; }
    int power() const { return this->totalNen.getEffectiveNenAbility() + this->getExp() + this->totalAura ; }

    bool isEmpty() const {
        return hunterNode == nullptr;
    }
};

class i_order {
private:
    int Aura;
    int squadId;
public:
    i_order(int Aura, int squadId) : Aura(Aura), squadId(squadId) {}

    int getAura() const { return Aura; }
    int getSquadId() const { return squadId; }
    void setAura(int Aura) { this->Aura = Aura; }
    void setSquadId(int squadId) { this->squadId = squadId; }

    bool operator<(const i_order& other) const {
        if (this->Aura < other.Aura) {
            return true;
        }
        if (this->Aura > other.Aura) {
            return false;
        }
        return this->squadId < other.squadId;
    }

    bool operator>(const i_order& other) const {
        return other < *this;
    }

    bool operator==(const i_order& other) const {
        return (this->Aura == other.Aura) && (this->squadId == other.squadId);
    }

    bool operator!=(const i_order& other) const {
        return !(*this == other);
    }
};