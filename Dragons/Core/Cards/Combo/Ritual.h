#pragma once

#include "../Card.h"
#include "../Offense/LastBreath.h"
#include "../Defense/LifeFlower.h"
#include "../Utility/LifeExchange.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/ActionEvents.h"

class Core;

namespace cards
{
    class Ritual : public Card
    {
        bool _resumeLifeExchange = false;
        bool _resumeToLastBreath = false;
        bool _resumeLastBreath = false;
        bool _resumeToLifeFlower = false;
        bool _resumeToCleanUp = false;

        std::unique_ptr<EventHandler<PostCardPlayedEvent>> _postCardPlayedHandler = nullptr;
        int _cardsPlayed = 0;

        const std::vector<CardId> _requiredCardIds
        {
            LastBreath::CARD_ID(),
            LifeFlower::CARD_ID(),
            LifeExchange::CARD_ID()
        };
        cards::Card* _cardLastBreath = nullptr;
        cards::Card* _cardLifeFlower = nullptr;
        cards::Card* _cardLifeExchange = nullptr;

    public:
        Ritual() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "ritual" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new Ritual()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Ritual"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}