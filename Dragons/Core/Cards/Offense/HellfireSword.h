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

        static CardId CARD_ID() { return { "hellfire_sword" }; }
        CardId GetCardId() const { return CARD_ID(); }

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Hellfire Sword"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}