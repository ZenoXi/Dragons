#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class StarCurse : public Card
    {
    public:
        StarCurse() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Star Curse"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}