#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class DesperateStrike : public Card
    {
    public:
        DesperateStrike() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Desperate Strike"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}