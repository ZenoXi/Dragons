#pragma once

#include "../Card.h"
#include "../Defense/WarBlessing.h"
#include "../Defense/Barrier.h"

class Core;

namespace cards
{
    class UltimateDefense : public Card
    {
        const std::vector<CardId> _requiredCardIds
        {
            WarBlessing::CARD_ID(),
            Barrier::CARD_ID()
        };
        cards::Card* _cardWarBlessing = nullptr;
        cards::Card* _cardBarrier = nullptr;

    public:
        UltimateDefense() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "ultimate_defense" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new UltimateDefense()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Ultimate Defense"; }
        std::wstring GetCardDescription() const { return L"War Blessing effect\nBarrier effect\nDraw a defense card."; }
        std::wstring GetCardFlavorText() const { return L"The best offense is good defense"; }
    };
}