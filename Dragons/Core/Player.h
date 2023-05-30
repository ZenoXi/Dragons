#pragma once

#include "Cards/Card.h"

#include <vector>
#include <memory>

struct Player
{
    int health;
    int maxHealth;
    int armor;
    int actionsLeft;
    int extraPlays;
    int extraDraws;
    std::vector<std::unique_ptr<Cards::Card>> hand;
    std::vector<std::unique_ptr<Cards::Card>> activeCards;
};