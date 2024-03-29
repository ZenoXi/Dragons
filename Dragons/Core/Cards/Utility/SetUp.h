#pragma once

#include "../Card.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/TurnEvents.h"

#include <memory>

class Core;

namespace cards
{
    struct SetUpPlayProperties : public PlayProperties
    {
        int actionCount = 4;
    };

    class SetUp : public Card
    {
        std::unique_ptr<EventHandler<TurnBeginEvent>> _turnBeginHandler = nullptr;
        int _actionCount;

    public:
        SetUp() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        bool IsActive() { return true; }

        static CardId CARD_ID() { return { "set_up" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new SetUp()); }

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Set Up"; }
        std::wstring GetCardDescription() const { return L"Next turn you can perform 4 actions instead of 2. (Doesn't stack with each other)"; }
        std::wstring GetCardFlavorText() const { return L"Preparing for the sacrifice, calm before the storm"; }

    private:
        void _OnEnterHand(Core* core, int playerIndex);
        void _OnEnterActiveCards(Core* core, int playerIndex);
        void _OnEnterDeck(Core* core);
        void _OnEnterGraveyard(Core* core);
        void _OnEnterDestroyedCards(Core* core);
    };
}