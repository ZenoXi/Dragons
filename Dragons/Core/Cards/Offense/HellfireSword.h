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

        CardType GetCardType() { return CardType::OFFENSE; }
        std::wstring GetCardName() { return L"Hellfire Sword"; }
        std::wstring GetCardDescription() { return L""; }
    };
}