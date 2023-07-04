#pragma once

#include "../Card.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/TurnEvents.h"
#include "../../Events/ActionEvents.h"

#include <memory>

class Core;

namespace cards
{
    class FireMoon : public Card
    {
        std::unique_ptr<EventHandler<TurnBeginEvent>> _turnBeginHandler = nullptr;
        std::unique_ptr<EventHandler<TurnEndEvent>> _turnEndHandler = nullptr;
        std::unique_ptr<EventHandler<CanPlayEvent>> _canPlayHandler = nullptr;
        bool _activated = false;
        int _targetPlayer = -1;

        bool _waitingForCardDraw = false;

    public:
        FireMoon() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);
        bool IsActive() { return true; }

        static CardId CARD_ID() { return { "fire_moon" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new FireMoon()); }

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Fire Moon"; }
        std::wstring GetCardDescription() const { return L""; }

    private:
        void _OnEnterHand(Core* core, int playerIndex);
        void _OnEnterActiveCards(Core* core, int playerIndex);
        void _OnEnterDeck(Core* core);
        void _OnEnterGraveyard(Core* core);
        void _OnEnterDestroyedCards(Core* core);
    };
}