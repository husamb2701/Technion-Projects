#pragma once

#include <deque>
#include <vector>
#include "Player.h"
#include "Event.h"
using namespace std;


class MatamStory {
private:
    unsigned int m_turnIndex = 0;
    vector<Player> m_players;
    deque<unique_ptr<Event>> m_events;

    void playTurn(Player& player);
    void playRound();
    bool isGameOver() const;

public:
    MatamStory(std::istream& eventsStream, std::istream& playersStream);
    void play();
    static bool comparePlayers(const Player& player1, const Player& player2);
    vector<Player> getLeaderBoard() const;
};
