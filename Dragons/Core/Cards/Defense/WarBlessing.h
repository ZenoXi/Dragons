#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class WarBlessing : public Card
    {
    public:
        WarBlessing() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "war_blessing" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new WarBlessing()); }

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"War Blessing"; }
        std::wstring GetCardDescription() const { return L"Increase max health by 5 and heal 1 health."; }
        std::wstring GetCardFlavorText() const { return L"You can feel the adrenaline in your veins"; }
    };
}