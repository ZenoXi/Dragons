#pragma once

namespace cards
{
    class Card;
}

struct DamageProperties
{
    int source = -1;
    int target = -1;
    int amount = 0;
    bool ignoreArmor = false;
    bool trueDamage = false;
    bool fatigue = false;
    cards::Card* sourceCard = nullptr;
};

struct DamageResult
{
    int removedArmorAmount = 0;
    int removedHealthAmount = 0;
};