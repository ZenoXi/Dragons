#pragma once

#include "../Card.h"
#include "../Offense/DevilsDeal.h"
#include "../Offense/BloodDonation.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/StatsEvents.h"

class Core;

namespace cards
{
    class Vampirism : public Card
    {
        bool _resumeBloodDonation = false;
        bool _resumeToDevilsDeal = false;
        bool _resumeDevilsDeal = false;
        bool _resumeToCardDraw = false;
        bool _waitingForCardDraw = false;
        bool _resumeToCleanUp = false;

        BloodDonationPlayProperties _bloodDonationPlayProperties;

        std::unique_ptr<EventHandler<PostDamageEvent>> _postDamageHandler = nullptr;
        int _damageToSelf;
        int _opponentDamageToSelf;

        const std::vector<CardId> _requiredCardIds
        {
            DevilsDeal::CARD_ID(),
            BloodDonation::CARD_ID()
        };
        cards::Card* _cardDevilsDeal = nullptr;
        cards::Card* _cardBloodDonation = nullptr;

    public:
        Vampirism()
        {
            // Make sure no cards will be drawn by blood donation itself and do it manually after Devils Deal is played
            _bloodDonationPlayProperties.healthForOneCard = 999;
        }

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "vampirism" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new Vampirism()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Vampirism"; }
        std::wstring GetCardDescription() const { return L"Devils Deal effect\nBlood Donation effect\nYou draw a card for each health lost and also for the damage opponent deals to himself."; }
        std::wstring GetCardFlavorText() const { return L"How much blood is enough blood?"; }
    };
}