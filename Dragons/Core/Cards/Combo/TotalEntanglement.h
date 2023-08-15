#pragma once

#include "../Card.h"
#include "../Offense/FireMoon.h"
#include "../Defense/SoothingSpell.h"
#include "../Utility/AbsoluteZero.h"

class Core;

namespace cards
{
    class TotalEntanglement : public Card
    {
        bool _resumeFireMoon = false;
        bool _resumeToSoothingSpell = false;

        const std::vector<CardId> _requiredCardIds
        {
            FireMoon::CARD_ID(),
            SoothingSpell::CARD_ID(),
            AbsoluteZero::CARD_ID()
        };
        cards::Card* _cardFireMoon = nullptr;
        cards::Card* _cardSoothingSpell = nullptr;
        cards::Card* _cardAbsoluteZero = nullptr;

    public:
        TotalEntanglement() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "total_entanglement" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new TotalEntanglement()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Total Entanglement"; }
        std::wstring GetCardDescription() const { return L"Absolute Zero effect\nFire Moon effect\nSoothing Spell effect\nYour opponent discards his hand. You can draw and play one more card this turn."; }
        std::wstring GetCardFlavorText() const { return L"Death awaits"; }
    };
}