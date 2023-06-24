#pragma once

#include "../Card.h"
#include "../Offense/FairFight.h"
#include "../Defense/TreeOfLife.h"
#include "../Utility/DragonEquation.h"

class Core;

namespace cards
{
    class AstroInfluence : public Card
    {
        bool _resumeSummonDead = false;
        bool _resumeToHelpingHand = false;
        bool _waitingToSelectCards = false;
        bool _resumeToCleanUp = false;

        const std::vector<CardId> _requiredCardIds
        {
            FairFight::CARD_ID(),
            TreeOfLife::CARD_ID(),
            DragonEquation::CARD_ID()
        };
        std::unique_ptr<cards::Card> _cardFairFight = nullptr;
        std::unique_ptr<cards::Card> _cardTreeOfLife = nullptr;
        std::unique_ptr<cards::Card> _cardDragonEquation = nullptr;

    public:
        AstroInfluence() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "astro_influence" }; }
        CardId GetCardId() const { return CARD_ID(); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Astro Influence"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}