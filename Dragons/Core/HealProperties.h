#pragma once

namespace cards
{
    class Card;
}

struct HealProperties
{
    int target = -1;
    int amount = -1;
    cards::Card* sourceCard = nullptr;
};