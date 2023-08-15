#pragma once

#include "../Card.h"
#include "../Offense/RapidAttack.h"
#include "../Offense/LastBreath.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/ActionEvents.h"

class Core;

namespace cards
{
    class FinalHour : public Card
    {
        bool _resumeLastBreath = false;
        bool _resumeToCleanUp = false;

        const std::vector<CardId> _requiredCardIds
        {
            RapidAttack::CARD_ID(),
            LastBreath::CARD_ID()
        };
        cards::Card* _cardRapidAttack = nullptr;
        cards::Card* _cardLastBreath = nullptr;

    public:
        FinalHour() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "final_hour" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new FinalHour()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Final Hour"; }
        std::wstring GetCardDescription() const { return L"Rapid Attack effect\nLast Breath\nYou don't have to discard any cards after using all offensive cards. If your hand is empty after using offensive cards, you win."; }
        std::wstring GetCardFlavorText() const { return L"This is it, the moment you've been waiting for"; }
    };
}