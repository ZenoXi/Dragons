#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class TheThief : public Card
    {
    public:
        TheThief() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"The Thief"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}