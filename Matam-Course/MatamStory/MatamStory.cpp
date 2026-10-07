#include "MatamStory.h"
#include "Utilities.h"
#include <algorithm>

#define MIN_PLAYERS 2
#define MAX_PLAYERS 6
#define MIN_EVENTS 2

using namespace std;

MatamStory::MatamStory(std::istream& eventsStream, std::istream& playersStream) {
    string line;
    while (getline(eventsStream, line)) {
        if (line.empty()) continue;
        unique_ptr<Event> event = EventFactory::createEvent(line);
        if (event) {
            m_events.push_back(move(event));
        }
    }
    string playerName, jobName, characterName;
    while (playersStream >> playerName >> jobName >> characterName) {
        unique_ptr<Job> job = JobFactory::createJob(jobName);
        unique_ptr<Character> character = CharacterFactory::createCharacter(characterName);
        Player player = PlayerFactory::createPlayer(playerName, move(job), move(character));
        m_players.push_back(player);
    }
    this->m_turnIndex = 0;
    if (m_players.size() < MIN_PLAYERS || m_players.size() > MAX_PLAYERS) {
        throw invalid_argument("Invalid Players File");
    }
    if (m_events.size() < MIN_EVENTS) {
        throw invalid_argument("Invalid Events File");
    }
}

void MatamStory::playTurn(Player& player) {
    unique_ptr<Event> currentEvent = move(m_events[0]);
    m_events.pop_front();
    printTurnDetails(this->m_turnIndex + 1, player, *currentEvent);
    string outcome = currentEvent->playEvent(player);
    m_events.push_back(move(currentEvent));
    printTurnOutcome(outcome);
    m_turnIndex++;
}

void MatamStory::playRound() {
    printRoundStart();
    for (Player& player : m_players) {
        if (!player.isDead()) {
            playTurn(player);
        }
    }
    printRoundEnd();
    printLeaderBoardMessage();
    vector<Player> leaderboard = getLeaderBoard();
    int rank = 1;
    for (const Player& player : leaderboard) {
        printLeaderBoardEntry(rank++, player);
    }
    printBarrier();
}

bool MatamStory::isGameOver() const {
    bool winnerExists = any_of(m_players.begin(), m_players.end(),
                        [](const Player& p) { return p.getLevel() == MAX_LEVEL; });
    if (winnerExists) return true;

    return all_of(m_players.begin(), m_players.end(),
                  [](const Player& p) { return p.isDead(); });
}

void MatamStory::play() {
    printStartMessage();
    for (unsigned int i = 0; i < m_players.size(); i++) {
        printStartPlayerEntry((i + 1), m_players[i]);
    }
    printBarrier();

    while (!isGameOver()) {
        playRound();
    }

    printGameOver();
    vector<Player> leaderboard = getLeaderBoard();
    if (leaderboard[0].getLevel() == MAX_LEVEL) {
        printWinner(leaderboard[0]);
    } else {
        printNoWinners();
    }
}

bool MatamStory::comparePlayers(const Player& player1, const Player& player2) {
    if (player1.getLevel() != player2.getLevel()) {
        return player1.getLevel() > player2.getLevel();
    }
    if (player1.getCoins() != player2.getCoins()) {
        return player1.getCoins() > player2.getCoins();
    }
    return player1.getName() < player2.getName();
}

vector<Player> MatamStory::getLeaderBoard() const {
    vector<Player> leaderBoard = m_players;
    sort(leaderBoard.begin(), leaderBoard.end(), comparePlayers);
    return leaderBoard;
}
