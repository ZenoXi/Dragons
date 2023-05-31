#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class DesperateStrike : public Card
    {
    public:
        DesperateStrike() {}

        bool CanPlay(Core* core, ActionProperties actionProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() { return CardType::OFFENSE; }
        std::wstring GetCardName() { return L"Desperate Strike"; }
        std::wstring GetCardDescription() { return L""; }
    };
}