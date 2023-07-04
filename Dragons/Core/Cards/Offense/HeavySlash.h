#pragma once

#include "../Card.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/TurnEvents.h"

#include <memory>

class Core;

namespace cards
{
    class HeavySlash : public Card
    {
        std::unique_ptr<EventHandler<TurnBeginEvent>> _turnBeginHandler = nullptr;
        int _playerIndex = -1;
        int _opponentIndex = -1;

    public:
        HeavySlash() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        bool IsActive() { return true; }

        static CardId CARD_ID() { return { "heavy_slash" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new HeavySlash()); }

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Heavy Slash"; }
        std::wstring GetCardDescription() const { return L""; }

    private:
        void _OnEnterHand(Core* core, int playerIndex);
        void _OnEnterActiveCards(Core* core, int playerIndex);
        void _OnEnterDeck(Core* core);
        void _OnEnterGraveyard(Core* core);
        void _OnEnterDestroyedCards(Core* core);
    };
}