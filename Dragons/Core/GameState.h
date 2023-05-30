#pragma once

#include "Cards/Card.h"
#include "Player.h"

#include <vector>
#include <memory>

struct GameState
{
    std::vector<Player> players;
    int currentPlayer = -1;

    std::vector<std::unique_ptr<Cards::Card>> offenseDeck;
    std::vector<std::unique_ptr<Cards::Card>> defenseDeck;
    std::vector<std::unique_ptr<Cards::Card>> utilityDeck;
    std::vector<std::unique_ptr<Cards::Card>> comboDeck;
    std::vector<std::unique_ptr<Cards::Card>> graveyard;
};