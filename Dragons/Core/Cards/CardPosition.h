#pragma once

namespace cards
{
    enum class CardSets
    {
        HAND,
        ACTIVE_CARDS,
        DECK,
        GRAVEYARD,
        IN_PLAY,
        DESTROYED,
        NONE
    };

    struct CardSet
    {
        CardSets set;
        int playerIndex = -1;
    };
}