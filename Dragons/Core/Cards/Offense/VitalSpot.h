#pragma once

#include "../Card.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/StatsEvents.h"

#include <memory>

class Core;

namespace cards
{
    struct VitalSpotPlayProperties : public PlayProperties
    {
        int blockedAttacks = 1;
    };

    class VitalSpot : public Card
    {
        std::unique_ptr<EventHandler<PreDamageEvent_BuffPass>> _preDamageHandler = nullptr;
        int _attacksToBlock;

    public:
        VitalSpot() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        bool IsActive() { return true; }

        static CardId CARD_ID() { return { "vital_spot" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new VitalSpot()); }

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Vital Spot"; }
        std::wstring GetCardDescription() const { return L""; }

    private:
        void _OnEnterHand(Core* core, int playerIndex);
        void _OnEnterActiveCards(Core* core, int playerIndex);
        void _OnEnterDeck(Core* core);
        void _OnEnterGraveyard(Core* core);
        void _OnEnterDestroyedCards(Core* core);
    };
}