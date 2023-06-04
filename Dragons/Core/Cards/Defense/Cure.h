#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class Cure : public Card
    {
    public:
        Cure() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Cure"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}