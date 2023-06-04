#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class WarBlessing : public Card
    {
    public:
        WarBlessing() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"War Blessing"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}