#pragma once

#include "../Card.h"
#include "../Offense/SacrificialAttack.h"
#include "../Offense/Stab.h"

class Core;

namespace cards
{
    class DoubleStrike : public Card
    {
        const std::vector<CardId> _requiredCardIds
        {
            SacrificialAttack::CARD_ID(),
            Stab::CARD_ID()
        };
        cards::Card* _cardSacrificialAttack = nullptr;
        cards::Card* _cardStab = nullptr;

    public:
        DoubleStrike() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "double_strike" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new DoubleStrike()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Double Strike"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}