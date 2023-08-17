#pragma once

#include "../Card.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/TurnEvents.h"
#include "../../Events/StatsEvents.h"

#include <memory>

class Core;

namespace cards
{
    class DeathPoison : public Card
    {
        std::unique_ptr<EventHandler<TurnEndEvent>> _turnEndHandler = nullptr;
        std::unique_ptr<EventHandler<PreDamageEvent_BuffPass>> _damageHandler = nullptr;
        _RelativeTarget _targetPlayer = _RelativeTarget::OPPONENT;

    public:
        DeathPoison() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        bool IsActive() { return true; }

        static CardId CARD_ID() { return { "death_poison" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new DeathPoison()); }

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Death Poison"; }
        std::wstring GetCardDescription() const { return L"Every offensive card this turn will deal 1 bonus damage to opponent."; }
        std::wstring GetCardFlavorText() const { return L"Everything becomes deadly with this on it"; }

    private:
        void _OnEnterHand(Core* core, int playerIndex);
        void _OnEnterActiveCards(Core* core, int playerIndex);
        void _OnEnterDeck(Core* core);
        void _OnEnterGraveyard(Core* core);
        void _OnEnterDestroyedCards(Core* core);
    };
}