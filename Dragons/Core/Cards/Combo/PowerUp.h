#pragma once

#include "../Card.h"
#include "../Defense/ArmorUp.h"
#include "../Defense/Healer.h"

class Core;

namespace cards
{
    class PowerUp : public Card
    {
        const std::vector<CardId> _requiredCardIds
        {
            ArmorUp::CARD_ID(),
            Healer::CARD_ID()
        };
        cards::Card* _cardArmorUp = nullptr;
        cards::Card* _cardHealer = nullptr;

    public:
        PowerUp() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "power_up" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new PowerUp()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Power Up"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}