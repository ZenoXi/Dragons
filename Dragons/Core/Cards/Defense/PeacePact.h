#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class PeacePact : public Card
    {
    public:
        PeacePact() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Peace Pact"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}