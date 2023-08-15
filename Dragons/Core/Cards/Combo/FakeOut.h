#pragma once

#include "../Card.h"
#include "../Defense/PeacePact.h"
#include "../Defense/Preparations.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/TurnEvents.h"
#include "../../Events/ActionEvents.h"

class Core;

namespace cards
{
    class FakeOut : public Card
    {
        bool _resumeFireMoon = false;
        bool _resumeToSoothingSpell = false;

        std::unique_ptr<EventHandler<TurnBeginEvent>> _turnBeginHandler = nullptr;
        std::unique_ptr<EventHandler<TurnEndEvent>> _turnEndHandler = nullptr;
        std::unique_ptr<EventHandler<CanDrawEvent>> _canDrawHandler = nullptr;
        bool _activated = false;

        const std::vector<CardId> _requiredCardIds
        {
            PeacePact::CARD_ID(),
            Preparations::CARD_ID()
        };
        cards::Card* _cardPeacePact = nullptr;
        cards::Card* _cardPreparations = nullptr;

    public:
        FakeOut() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        bool IsActive() { return true; }

        static CardId CARD_ID() { return { "fake_out" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new FakeOut()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Fake Out"; }
        std::wstring GetCardDescription() const { return L"Peace Pact effect\nPreparations effect\nOnly opponent discards his offensive cards. Opponent can't draw offensive cards next turn."; }
        std::wstring GetCardFlavorText() const { return L"Little did he know, you were not defenseless"; }

    private:
        void _OnEnterHand(Core* core, int playerIndex);
        void _OnEnterActiveCards(Core* core, int playerIndex);
        void _OnEnterDeck(Core* core);
        void _OnEnterGraveyard(Core* core);
        void _OnEnterDestroyedCards(Core* core);
    };
}