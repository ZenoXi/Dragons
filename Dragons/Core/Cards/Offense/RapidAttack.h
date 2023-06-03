#pragma once

#include "../Card.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/TurnEvents.h"

#include <memory>

class Core;

namespace cards
{
    class RapidAttack : public Card
    {
        std::unique_ptr<EventHandler<TurnEndEvent>> _turnEndHandler = nullptr;
        bool _playedThisTurn = false;
        int _playedBy = -1;

    public:
        RapidAttack() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        //void Draw(Core* core, ActionProperties actionProps);
        bool IsActive() { return true; }

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Rapid Attack"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}