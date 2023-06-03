#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class HellfireSword : public Card
    {
    public:
        HellfireSword() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Hellfire Sword"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}