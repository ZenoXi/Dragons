#include "DivineProtection.h"

#include "../../Core.h"

cards::PlayResult cards::DivineProtection::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    // Move card to active
    auto cardPtr = core->RemoveCardFromHand(this, actionProps.player);
    core->AddCardToActiveCards(std::move(cardPtr), actionProps.player);

    return PlayResult::DontDiscard();
}

void cards::DivineProtection::_OnEnterHand(Core* core, int playerIndex)
{
    _turnBeginHandler.reset();
    _turnEndHandler.reset();
    _healthChangeHandler.reset();
    _activated = false;
}

void cards::DivineProtection::_OnEnterActiveCards(Core* core, int playerIndex)
{
    _activated = false;

    _turnBeginHandler = std::make_unique<EventHandler<TurnBeginEvent>>(&core->Events(), [=](TurnBeginEvent event)
    {
        if (event.opponentIndex != GetPosition().playerIndex)
            return;

        _activated = true;
    });
    _turnEndHandler = std::make_unique<EventHandler<TurnEndEvent>>(&core->Events(), [=](TurnEndEvent event)
    {
        if (!_activated)
            return;

        auto cardPtr = core->RemoveCardFromActiveCards(this, event.opponentIndex);
        core->AddCardToGraveyard(std::move(cardPtr));
    });
    _healthChangeHandler = std::make_unique<EventHandler<PreHealthChangeEvent>>(&core->Events(), [=](PreHealthChangeEvent event)
    {
        if (!_activated)
            return;
        if (event.target != GetPosition().playerIndex)
            return;

        if (*event.newValue < 1)
            *event.newValue = 1;
    });
}

void cards::DivineProtection::_OnEnterDeck(Core* core)
{
    _turnBeginHandler.reset();
    _turnEndHandler.reset();
    _healthChangeHandler.reset();
    _activated = false;
}

void cards::DivineProtection::_OnEnterGraveyard(Core* core)
{
    _turnBeginHandler.reset();
    _turnEndHandler.reset();
    _healthChangeHandler.reset();
    _activated = false;
}

void cards::DivineProtection::_OnEnterDestroyedCards(Core* core)
{
    _turnBeginHandler.reset();
    _turnEndHandler.reset();
    _healthChangeHandler.reset();
    _activated = false;
}