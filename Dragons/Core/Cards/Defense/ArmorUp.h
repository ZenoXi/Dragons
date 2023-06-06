#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class ArmorUp : public Card
    {
    public:
        ArmorUp() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "armor_up" }; }
        CardId GetCardId() const { return CARD_ID(); }

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Armor Up"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}