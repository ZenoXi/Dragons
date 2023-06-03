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
        std::unique_ptr<EventHandler<PreDamageEvent>> _damageHandler = nullptr;

    public:
        DeathPoison() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        bool IsActive() { return true; }

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Death Poison"; }
        std::wstring GetCardDescription() const { return L""; }

    private:
        void _OnEnterHand(Core* core, int playerIndex);
        void _OnEnterActiveCards(Core* core, int playerIndex);
        void _OnEnterDeck(Core* core);
        void _OnEnterGraveyard(Core* core);
    };
}