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

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Life Flower"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}