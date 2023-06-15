#pragma once

namespace cards
{
    struct CardPosition
    {
        enum class Set
        {
            HAND,
            ACTIVE_CARDS,
            DECK,
            GRAVEYARD,
            DESTROYED
        };

        Set set;
        int playerIndex;
    };
}