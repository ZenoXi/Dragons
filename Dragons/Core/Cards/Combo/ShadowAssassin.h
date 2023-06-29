#pragma once

#include "../Card.h"
#include "../Offense/VitalSpot.h"
#include "../Offense/DeathPoison.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/StatsEvents.h"
#include "../../Events/TurnEvents.h"

class Core;

namespace cards
{
    class ShadowAssassin : public Card
    {
        const std::vector<CardId> _requiredCardIds
        {
            VitalSpot::CARD_ID(),
            DeathPoison::CARD_ID()
        };
        cards::Card* _cardVitalSpot = nullptr;
        cards::Card* _cardDeathPoison = nullptr;

        struct _ComboEffect
        {
            std::unique_ptr<EventHandler<PreDamageEvent_BuffPass>> preDamageHandler = nullptr;
            std::unique_ptr<EventHandler<TurnEndEvent>> turnEndHandler = nullptr;
        };
        std::vector<std::unique_ptr<_ComboEffect>> _activeComboEffects;

    public:
        ShadowAssassin() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "shadow_assassin" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new ShadowAssassin()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Shadow Assassin"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}