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

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Sacrificial Attack"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}