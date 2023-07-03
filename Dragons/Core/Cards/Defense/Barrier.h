#pragma once

#include "../Card.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/StatsEvents.h"
#include "../../Events/ActionEvents.h"

#include "../Utility/DragonFlame.h"

#include <memory>

class Core;

namespace cards
{
    struct BarrierPlayProperties : public PlayProperties
    {
        int attacksToBlock = 1;
        bool blockDragonFlame = false;
    };

    class Barrier : public Card
    {
        std::unique_ptr<EventHandler<PreDamageEvent_NerfPass>> _preDamageHandler = nullptr;
        std::unique_ptr<EventHandler<PreCardPlayedEvent>> _preCardPlayedHandler = nullptr;
        int _attacksToBlock;
        bool _blockDragonFlame;
        DragonFlamePlayProps _dragonFlameBlockPlayProps;

    public:
        Barrier() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        bool IsActive() { return true; }

        static CardId CARD_ID() { return { "barrier" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new Barrier()); }

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Barrier"; }
        std::wstring GetCardDescription() const { return L""; }

    private:
        void _OnEnterHand(Core* core, int playerIndex);
        void _OnEnterActiveCards(Core* core, int playerIndex);
        void _OnEnterDeck(Core* core);
        void _OnEnterGraveyard(Core* core);
        void _OnEnterDestroyedCards(Core* core);
    };
}