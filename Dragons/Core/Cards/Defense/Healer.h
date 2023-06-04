#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class Healer : public Card
    {
    public:
        Healer() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Healer"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}