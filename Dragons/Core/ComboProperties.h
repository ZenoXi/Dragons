#pragma once

#include "Cards/CardId.h"

#include <vector>
#include <string>

struct ComboProperties
{
    std::vector<CardId> requiredCards;
    int player;
};