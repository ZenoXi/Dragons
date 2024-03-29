#pragma once

#include "../Card.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/StatsEvents.h"

#include <memory>

class Core;

namespace cards
{
    class SpiritShelter : public Card
    {
        std::unique_ptr<EventHandler<PreDamageEvent_NerfPass>> _damageHandler = nullptr;

    public:
        SpiritShelter() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        bool IsActive() { return true; }

        static CardId CARD_ID() { return { "spirit_shelter" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new SpiritShelter()); }

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Spirit Shelter"; }
        std::wstring GetCardDescription() const { return L"You don't take fatigue damage."; }
        std::wstring GetCardFlavorText() const { return L"You realise, you finally found a place to rest"; }

    private:
        void _OnEnterHand(Core* core, int playerIndex);
        void _OnEnterActiveCards(Core* core, int playerIndex);
        void _OnEnterDeck(Core* core);
        void _OnEnterGraveyard(Core* core);
        void _OnEnterDestroyedCards(Core* core);
    };
}