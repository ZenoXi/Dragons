#pragma once

#include "../Card.h"
#include "../Offense/BloodDonation.h"
#include "../Defense/DivineProtection.h"
#include "../Utility/FrostFire.h"

class Core;

namespace cards
{
    class InfernoLifeline : public Card
    {
        bool _resumeFrostFire = false;
        bool _resumeToBloodDonation = false;
        bool _resumeBloodDonation = false;
        bool _resumeToDivineProtection = false;
        bool _resumeToComboEffect = false;
        bool _waitingForDeckChoice = false;
        bool _waitingForCardChoice = false;
        bool _resumeToCleanUp = false;

        CardType _chosenDeck;

        const std::vector<CardId> _requiredCardIds
        {
            BloodDonation::CARD_ID(),
            DivineProtection::CARD_ID(),
            FrostFire::CARD_ID()
        };
        cards::Card* _cardBloodDonation = nullptr;
        cards::Card* _cardDivineProtection = nullptr;
        cards::Card* _cardFrostFire = nullptr;

    public:
        InfernoLifeline() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "inferno_lifeline" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new InfernoLifeline()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Inferno Lifeline"; }
        std::wstring GetCardDescription() const { return L"Frost Fire effect\nBlood Donation effect\nDivine Protection effect\nReduce your health to one. Search and draw any amount of cards from one deck. Discard the rest of the deck afterwards."; }
        std::wstring GetCardFlavorText() const { return L"What doesn't kill you, makes you stronger"; }
    };
}