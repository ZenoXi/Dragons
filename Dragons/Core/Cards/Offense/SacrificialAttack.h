#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class SacrificialAttack : public Card
    {
    public:
        SacrificialAttack() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() { return CardType::OFFENSE; }
        std::wstring GetCardName() { return L"Sacrificial Attack"; }
        std::wstring GetCardDescription() { return L""; }
    };
}