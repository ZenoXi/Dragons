#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    struct LifeFlowerPlayProperties : public PlayProperties
    {
        int maxHealthIncrease = 1;
        int healAmount = 1;
        int armorAmount = 1;
    };

    class LifeFlower : public Card
    {
    public:
        LifeFlower() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "life_flower" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new LifeFlower()); }

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Life Flower"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}