#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class LifeFlower : public Card
    {
    public:
        LifeFlower() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "life_flower" }; }
        CardId GetCardId() const { return CARD_ID(); }

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Life Flower"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}