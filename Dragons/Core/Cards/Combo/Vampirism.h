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
        Vampirism() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "vampirism" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new Vampirism()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Vampirism"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}