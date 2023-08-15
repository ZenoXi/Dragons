#pragma once

#include "../Card.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/ActionEvents.h"

#include <memory>

class Core;

namespace cards
{
    class CorpsePuppet : public Card
    {
        std::unique_ptr<EventHandler<CanPlayComboEvent>> _canPlayComboHandler = nullptr;
        std::unique_ptr<EventHandler<PreGetComboCardsEvent>> _preGetComboCardsHandler = nullptr;

    public:
        CorpsePuppet() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        bool IsActive() { return true; }

        static CardId CARD_ID() { return { "corpse_puppet" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new CorpsePuppet()); }

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Corpse Puppet"; }
        std::wstring GetCardDescription() const { return L"For the next combo you can use a card from the graveyard."; }
        std::wstring GetCardFlavorText() const { return L"No dead man is useless"; }

    private:
        void _OnEnterHand(Core* core, int playerIndex);
        void _OnEnterActiveCards(Core* core, int playerIndex);
        void _OnEnterDeck(Core* core);
        void _OnEnterGraveyard(Core* core);
        void _OnEnterDestroyedCards(Core* core);
    };
}