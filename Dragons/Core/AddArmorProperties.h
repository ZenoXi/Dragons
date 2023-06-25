#pragma once

namespace cards
{
    class Card;
}

struct AddArmorProperties
{
    int target = -1;
    int amount = -1;
    cards::Card* sourceCard = nullptr;
};