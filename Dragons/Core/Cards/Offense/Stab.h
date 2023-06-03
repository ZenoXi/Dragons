#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class Stab : public Card
    {
    public:
        Stab() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Stab"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}