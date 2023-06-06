#pragma once

#include "../Card.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/TurnEvents.h"

#include <memory>

class Core;

namespace cards
{
    class FairFight : public Card
    {
        std::unique_ptr<EventHandler<TurnEndEvent>> _turnEndHandler = nullptr;

    public:
        FairFight() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        bool IsActive() { return true; }

        static CardId CARD_ID() { return { "fair_fight" }; }
        CardId GetCardId() const { return CARD_ID(); }

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Fair Fight"; }
        std::wstring GetCardDescription() const { return L""; }

    private:
        void _OnEnterHand(Core* core, int playerIndex);
        void _OnEnterActiveCards(Core* core, int playerIndex);
        void _OnEnterDeck(Core* core);
        void _OnEnterGraveyard(Core* core);
        void _OnEnterDestroyedCards(Core* core);
    };
}