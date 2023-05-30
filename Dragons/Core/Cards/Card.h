#pragma once

#include <string>

class Core;

namespace cards
{
    struct ActionProperties
    {
        int player;
        int opponent;
    };

    // Used to pass customization data to played card
    struct PlayProperties {};

    struct PlayResult
    {
        bool discard = true;
    };

    enum class CardType
    {
        OFFENSE,
        DEFENSE,
        UTILITY,
        COMBO
    };

    class Card
    {
    public:
        virtual bool CanPlay(Core* core, ActionProperties actionProps) = 0;
        virtual PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps) = 0;
        virtual bool IsActive() { return false; }

        // Metadata
        virtual CardType GetCardType() = 0;
        virtual std::wstring GetCardName() = 0;
        virtual std::wstring GetCardDescription() = 0;
    };
}
