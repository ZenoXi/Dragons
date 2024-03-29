#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    struct HellfireSwordPlayProperties : public PlayProperties
    {
        bool stealArmor = false;
    };

    class HellfireSword : public Card
    {
    public:
        HellfireSword() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "hellfire_sword" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new HellfireSword()); }

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Hellfire Sword"; }
        std::wstring GetCardDescription() const { return L"Destroy enemy armor."; }
        std::wstring GetCardFlavorText() const { return L"Just a touch from this sword will melt any metal"; }
    };
}