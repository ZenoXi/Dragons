#pragma once

#include "../Card.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/TurnEvents.h"
#include "../../Events/StatsEvents.h"

#include <memory>

class Core;

namespace cards
{
    class DivineProtection : public Card
    {
        std::unique_ptr<EventHandler<TurnBeginEvent>> _turnBeginHandler = nullptr;
        std::unique_ptr<EventHandler<TurnEndEvent>> _turnEndHandler = nullptr;
        std::unique_ptr<EventHandler<PreHealthChangeEvent>> _healthChangeHandler = nullptr;
        bool _activated = false;

    public:
        DivineProtection() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        bool IsActive() { return true; }

        static CardId CARD_ID() { return { "divine_protection" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new DivineProtection()); }

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Divine Protection"; }
        std::wstring GetCardDescription() const { return L"You cannot be reduced below 1 health next turn."; }
        std::wstring GetCardFlavorText() const { return L"A godly protective spell"; }

    private:
        void _OnEnterHand(Core* core, int playerIndex);
        void _OnEnterActiveCards(Core* core, int playerIndex);
        void _OnEnterDeck(Core* core);
        void _OnEnterGraveyard(Core* core);
        void _OnEnterDestroyedCards(Core* core);
    };
}