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
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new DragonEquation()); }

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Dragon Equation"; }
        std::wstring GetCardDescription() const { return L"Swap both players active cards."; }
        std::wstring GetCardFlavorText() const { return L"Taking from the strong, giving to the stronger"; }
    };
}