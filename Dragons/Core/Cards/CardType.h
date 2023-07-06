#pragma once

#include <string>

namespace cards
{
    enum class CardType
    {
        OFFENSE = 0,
        DEFENSE = 1,
        UTILITY = 2,
        COMBO = 3,
        NONE = -1
    };

    std::string CardTypeToString(CardType type);
}