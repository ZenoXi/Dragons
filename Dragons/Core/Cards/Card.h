#pragma once

#include "../ActionProperties.h"

#include <string>

class Core;

namespace cards
{
    // Used to pass customization data to played card
    struct PlayProperties {};

    struct PlayResult
    {
        bool discard = true;
        bool notPlayed = false;
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
        virtual bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps) { return true; }
        virtual PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps) = 0;
        virtual void Draw(Core* core, ActionProperties actionProps) {}
        virtual void Discard(Core* core, ActionProperties actionProps) {}
        virtual bool IsActive() { return false; }
        virtual int GetActionCost() { return 1; }

        // Metadata
        virtual CardType GetCardType() = 0;
        virtual std::wstring GetCardName() = 0;
        virtual std::wstring GetCardDescription() = 0;
    };
}
