#pragma once

#include "Cards/Card.h"
#include "Player.h"

#include <vector>
#include <memory>

struct GameState
{
    std::vector<Player> players;
    int currentPlayer = -1;
    int opposingPlayer = -1;

    std::vector<std::unique_ptr<cards::Card>> offenseDeck;
    std::vector<std::unique_ptr<cards::Card>> defenseDeck;
    std::vector<std::unique_ptr<cards::Card>> utilityDeck;
    std::vector<std::unique_ptr<cards::Card>> comboDeck;
    std::vector<std::unique_ptr<cards::Card>> graveyard;
};