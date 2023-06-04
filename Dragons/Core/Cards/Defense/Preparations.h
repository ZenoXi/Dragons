#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class Preparations : public Card
    {
    public:
        Preparations() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Preparations"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}