#include "Huntech26a2.h"
#include "HunterNode.h"

Huntech::Huntech() = default;

Huntech::~Huntech() = default;

StatusType Huntech::add_squad(int squadId) {
    //checking validity
    if (squadId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (squadTree.search(squadId) != nullptr) {
        return StatusType::FAILURE;
    }
    //inserting to trees
    try {
        std::shared_ptr<Squad> newSquad = std::make_shared<Squad>(squadId);
        if (!squadTree.insert(squadId, newSquad)) {
            return StatusType::ALLOCATION_ERROR;
        }
        try {
            iTree.insert(i_order(0, squadId), newSquad);
        } catch (...) {
            //rollback
            squadTree.remove(squadId);
            return StatusType::ALLOCATION_ERROR;
        }
        return StatusType::SUCCESS;
    } catch (const std::bad_alloc &) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Huntech::remove_squad(int squadId) {
    //checking validity
    if (squadId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Squad> *squadPtr = squadTree.search(squadId);
    if (squadPtr == nullptr) {
        return StatusType::FAILURE;
    }
    std::shared_ptr<Squad> squad = *squadPtr;
    if (!squad->isEmpty()) {
        //inserting data to root
        HunterNode* leader = squad->getHunterNode();
        if (leader != nullptr) {
            leader->setSquad(std::weak_ptr<Squad>());
            leader->setDeltaFights(leader->getDeltaFights() + squad->getFightsOfSquad());
        }
    }
    //removing from trees
    i_order key(squad->getTotalAura(), squadId);
    iTree.remove(key);
    squadTree.remove(squadId);
    return StatusType::SUCCESS;
}

StatusType Huntech::add_hunter(int hunterId,
                               int squadId,
                               const NenAbility &nenType,
                               int aura,
                               int fightsHad) {
    //checking validity
    if (hunterId <= 0 || squadId <= 0 || aura < 0 || fightsHad < 0 || !nenType.isValid()) {
        return StatusType::INVALID_INPUT;
    }
    if (hunterMap.find(hunterId) != nullptr) {
        return StatusType::FAILURE;
    }
    std::shared_ptr<Squad> *squadPtr = squadTree.search(squadId);
    if (squadPtr == nullptr) {
        return StatusType::FAILURE;
    }
    //making hunter
    std::shared_ptr<Squad>& squad = *squadPtr;
    std::shared_ptr<HunterNode> newHunterNode;
    try {
        newHunterNode = std::make_shared<HunterNode>(hunterId, nenType, aura, fightsHad);
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
    StatusType insertStatus = hunterMap.insert(hunterId, newHunterNode);
    if (insertStatus != StatusType::SUCCESS) {
        return insertStatus;
    }
    //removing from iTree
    i_order oldKey(squad->getTotalAura(), squad->getSquadId());
    iTree.remove(oldKey);
    //set hunter to squad
    if (squad->isEmpty()) {
        squad->setHunterNode(newHunterNode.get());
        newHunterNode->setSquad(squad);
    } else {
        HunterNode::unite(squad->getHunterNode(), newHunterNode.get(), squad);
    }
    //inserting to iTree with new key
    squad->setTotalAura(squad->getTotalAura() + aura);
    squad->setTotalNen(squad->getTotalNen() + nenType);
    i_order newKey(squad->getTotalAura(), squad->getSquadId());
    try {
        iTree.insert(newKey, squad);
    } catch (...) {
        //rollback
        squad->setTotalAura(squad->getTotalAura() - aura);
        squad->setTotalNen(squad->getTotalNen() - nenType);
        hunterMap.remove(hunterId);
        try {
            iTree.insert(oldKey, squad);
        } catch (...) {
        }
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

output_t<int> Huntech::squad_duel(int squadId1, int squadId2) {
    //checking validity
    if (squadId1 == squadId2 || squadId1 <= 0 || squadId2 <= 0) {
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Squad> *SquadPtr1 = squadTree.search(squadId1);
    std::shared_ptr<Squad> *SquadPtr2 = squadTree.search(squadId2);
    if (SquadPtr1 == nullptr || SquadPtr2 == nullptr) {
        return StatusType::FAILURE;
    }
    std::shared_ptr<Squad> &squad1 = *SquadPtr1;
    std::shared_ptr<Squad> &squad2 = *SquadPtr2;
    if (squad1->isEmpty() || squad2->isEmpty()) {
        return StatusType::FAILURE;
    }
    //calculating total power
    int power1 = squad1->getTotalAura() + squad1->getExp();
    int power2 = squad2->getTotalAura() + squad2->getExp();
    //updating teams
    squad1->setFightsOfSquad(squad1->getFightsOfSquad() + 1);
    squad2->setFightsOfSquad(squad2->getFightsOfSquad() + 1);
    if (power1 > power2) {
        squad1->setExp(squad1->getExp() + 3);
        return 1;
    } else if (power2 > power1) {
        squad2->setExp(squad2->getExp() + 3);
        return 3;
    } else if (squad1->getTotalNen() > squad2->getTotalNen()) {
        squad1->setExp(squad1->getExp() + 3);
        return 2;
    } else if (squad2->getTotalNen() > squad1->getTotalNen()) {
        squad2->setExp(squad2->getExp() + 3);
        return 4;
    } else {
        squad1->setExp(squad1->getExp() + 1);
        squad2->setExp(squad2->getExp() + 1);
        return 0;
    }
}

output_t<int> Huntech::get_hunter_fights_number(int hunterId) {
    //checking validity
    if (hunterId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    auto hunterPtr = hunterMap.find(hunterId);
    if (hunterPtr == nullptr) {
        return StatusType::FAILURE;
    }
    //making raw pointer
    HunterNode *hunter = hunterPtr->get();
    //finding squad root and shrinking the tree to update deltaFights
    HunterNode *root = HunterNode::find(hunter);
    //calculating fights number
    int squadFightsNumber = 0;
    std::shared_ptr<Squad> currentSquad = root->getSquad().lock();//making weakPtr to sharePtr
    if (currentSquad) { //checking if squad still exist
        squadFightsNumber = currentSquad->getFightsOfSquad();
    }
    int totalFightsNumber = hunter->getCurrentFights() + hunter->getDeltaFights() + squadFightsNumber;
    if (hunter != root) {
        totalFightsNumber += root->getDeltaFights();
    }
    return totalFightsNumber;
}

output_t<int> Huntech::get_squad_experience(int squadId) {
    //checking validity
    if (squadId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    //find the Exp
    std::shared_ptr<Squad> *SquadPtr = squadTree.search(squadId);
    if (SquadPtr != nullptr) {
        return (*SquadPtr)->getExp();
    } else {
        return StatusType::FAILURE;
    }
}

output_t<int> Huntech::get_ith_collective_aura_squad(int i) {
    //checking validity
    if (i < 1) {
        return StatusType::FAILURE;
    }
    //finding the team
    std::shared_ptr<Squad> *ithSquadPtr = iTree.select(i);
    if (ithSquadPtr == nullptr) {
        return StatusType::FAILURE;
    }
    //returning squadId
    std::shared_ptr<Squad> &ithSquad = *ithSquadPtr;
    return ithSquad->getSquadId();
}

output_t<NenAbility> Huntech::get_partial_nen_ability(int hunterId) {
    //checking validity
    if (hunterId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    auto hunterPtr = hunterMap.find(hunterId);
    if (hunterPtr == nullptr) {
        return StatusType::FAILURE;
    }
    HunterNode *hunter = hunterPtr->get();
    HunterNode *root = HunterNode::find(hunter);
    if (root->getSquad().expired()) {
        return StatusType::FAILURE;
    }
    NenAbility result = hunter->getAbility() + hunter->getDeltaNen();
    if (hunter != root) {
        result += root->getDeltaNen();
    }
    return result;
}

StatusType Huntech::force_join(int forcingSquadId, int forcedSquadId) {
    if (forcingSquadId <= 0 || forcedSquadId <= 0 || forcingSquadId == forcedSquadId) {
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Squad> *forcingPtr = squadTree.search(forcingSquadId);
    std::shared_ptr<Squad> *forcedPtr = squadTree.search(forcedSquadId);
    if (forcingPtr == nullptr || forcedPtr == nullptr) {
        return StatusType::FAILURE;
    }
    std::shared_ptr<Squad> &forcingSquad = *forcingPtr;
    std::shared_ptr<Squad> &forcedSquad = *forcedPtr;
    if (forcingSquad->isEmpty()) {
        return StatusType::FAILURE;
    }
    if (forcedSquad->power() >= forcingSquad->power()) {
        return StatusType::FAILURE;
    }
    HunterNode *nodeA = forcingSquad->getHunterNode();
    HunterNode *nodeB = forcedSquad->getHunterNode();
    if (nodeA == nullptr) {
        return StatusType::FAILURE;
    }
    iTree.remove(i_order(forcingSquad->getTotalAura(), forcingSquadId));
    iTree.remove(i_order(forcedSquad->getTotalAura(), forcedSquadId));
    HunterNode::unite(nodeA, nodeB, forcingSquad);
    forcingSquad->setExp(forcingSquad->getExp() + forcedSquad->getExp());
    forcingSquad->setTotalAura(forcingSquad->getTotalAura() + forcedSquad->getTotalAura());
    forcingSquad->setTotalNen(forcingSquad->getTotalNen() + forcedSquad->getTotalNen());
    squadTree.remove(forcedSquadId);
    try {
        iTree.insert(i_order(forcingSquad->getTotalAura(), forcingSquadId), forcingSquad);
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}