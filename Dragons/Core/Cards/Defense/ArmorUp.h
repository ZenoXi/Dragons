#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    struct ArmorUpPlayProperties : public PlayProperties
    {
        int armorAmount = 2;
    };

    class ArmorUp : public Card
    {
    public:
        ArmorUp() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "armor_up" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new ArmorUp()); }

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Armor Up"; }
        std::wstring GetCardDescription() const { return L"Gain 2 armor."; }
        std::wstring GetCardFlavorText() const { return L"Blessed titanium armor set"; }
    };
}