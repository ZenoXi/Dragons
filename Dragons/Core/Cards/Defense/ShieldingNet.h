#pragma once

#include "../Card.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/StatsEvents.h"

#include <memory>

class Core;

namespace cards
{
    class ShieldingNet : public Card
    {
        std::unique_ptr<EventHandler<PreDamageEvent_NerfPass>> _damageHandler = nullptr;

    public:
        ShieldingNet() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        bool IsActive() { return true; }

        static CardId CARD_ID() { return { "shielding_net" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new ShieldingNet()); }

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Shielding Net"; }
        std::wstring GetCardDescription() const { return L"Reduces all damage by one down to a minimum of 1 damage."; }
        std::wstring GetCardFlavorText() const { return L"Even the slightest defense can go a long way"; }

    private:
        void _OnEnterHand(Core* core, int playerIndex);
        void _OnEnterActiveCards(Core* core, int playerIndex);
        void _OnEnterDeck(Core* core);
        void _OnEnterGraveyard(Core* core);
        void _OnEnterDestroyedCards(Core* core);
    };
}