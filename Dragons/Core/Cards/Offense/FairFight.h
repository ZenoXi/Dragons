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

        CardType GetCardType() { return CardType::OFFENSE; }
        std::wstring GetCardName() { return L"Fair Fight"; }
        std::wstring GetCardDescription() { return L""; }
    };
}