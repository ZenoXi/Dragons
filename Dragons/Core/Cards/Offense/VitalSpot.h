#pragma once

#include "../Card.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/StatsEvents.h"

#include <memory>

class Core;

namespace cards
{
    class VitalSpot : public Card
    {
        std::unique_ptr<EventHandler<PreDamageEvent>> _preDamageHandler = nullptr;

    public:
        VitalSpot() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        bool IsActive() { return true; }

        CardType GetCardType() { return CardType::OFFENSE; }
        std::wstring GetCardName() { return L"Vital Spot"; }
        std::wstring GetCardDescription() { return L""; }
    };
}