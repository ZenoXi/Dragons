#pragma once

#include "../Card.h"
#include "../Offense/DesperateStrike.h"
#include "../Defense/Healer.h"
#include "../Utility/HiddenTreasures.h"

class Core;

namespace cards
{
    class DifferentSides : public Card
    {
        bool _resumeHiddenTreasures = false;
        bool _resumeToHealer = false;

        const std::vector<CardId> _requiredCardIds
        {
            DesperateStrike::CARD_ID(),
            Healer::CARD_ID(),
            HiddenTreasures::CARD_ID()
        };
        cards::Card* _cardDesperateStrike = nullptr;
        cards::Card* _cardHealer = nullptr;
        cards::Card* _cardHiddenTreasures = nullptr;

    public:
        DifferentSides() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "different_sides" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new DifferentSides()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Different Sides"; }
        std::wstring GetCardDescription() const { return L"Hidden Treasure effect\nDesperate Strike effect\nHealer effect\nIf you have less cards than your opponent, damage increased to 5. If after drawing you have more cards than your opponent, heal increased to 5."; }
        std::wstring GetCardFlavorText() const { return L"Which side will win?"; }
    };
}