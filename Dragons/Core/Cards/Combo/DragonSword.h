#pragma once

#include "../Card.h"
#include "../Offense/HellfireSword.h"
#include "../Offense/HeavySlash.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/TurnEvents.h"

class Core;

namespace cards
{
    class DragonSword : public Card
    {
        std::unique_ptr<EventHandler<TurnBeginEvent>> _turnBeginHandler = nullptr;

        const std::vector<CardId> _requiredCardIds
        {
            HellfireSword::CARD_ID(),
            HeavySlash::CARD_ID()
        };
        cards::Card* _cardHellfireSword = nullptr;
        cards::Card* _cardHeavySlash = nullptr;

    public:
        DragonSword() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        bool IsActive() { return true; }

        static CardId CARD_ID() { return { "dragon_sword" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new DragonSword()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Dragon Sword"; }
        std::wstring GetCardDescription() const { return L""; }

    private:
        void _OnEnterHand(Core* core, int playerIndex);
        void _OnEnterActiveCards(Core* core, int playerIndex);
        void _OnEnterDeck(Core* core);
        void _OnEnterGraveyard(Core* core);
        void _OnEnterDestroyedCards(Core* core);
    };
}