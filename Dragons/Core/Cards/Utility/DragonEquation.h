#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class DragonEquation : public Card
    {
    public:
        DragonEquation() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "dragon_equation" }; }
        CardId GetCardId() const { return CARD_ID(); }

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Dragon Equation"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}