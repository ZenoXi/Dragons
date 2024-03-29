#include "DeathPoison.h"

#include "../../Core.h"

cards::PlayResult cards::DeathPoison::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (actionProps.opponent == actionProps.player)
        _targetPlayer = _RelativeTarget::OWNER;

    return PlayResult::AddToActives();
}

void cards::DeathPoison::_OnEnterHand(Core* core, int playerIndex)
{
    _turnEndHandler.reset();
    _damageHandler.reset();
}

void cards::DeathPoison::_OnEnterActiveCards(Core* core, int playerIndex)
{
    _turnEndHandler = std::make_unique<EventHandler<TurnEndEvent>>(&core->Events(), [=](TurnEndEvent event)
    {
        auto cardPtr = core->RemoveCardFromActiveCards(this, GetPosition().playerIndex);
        core->AddCardToGraveyard(std::move(cardPtr));
    });
    _damageHandler = std::make_unique<EventHandler<PreDamageEvent_BuffPass>>(&core->Events(), [=](PreDamageEvent_BuffPass event)
    {
        if (event.props->target == _TargetPlayerIndex(_targetPlayer))
            event.props->amount++;
    });
}

void cards::DeathPoison::_OnEnterDeck(Core* core)
{
    _turnEndHandler.reset();
    _damageHandler.reset();
}

void cards::DeathPoison::_OnEnterGraveyard(Core* core)
{
    _turnEndHandler.reset();
    _damageHandler.reset();
}

void cards::DeathPoison::_OnEnterDestroyedCards(Core* core)
{
    _turnEndHandler.reset();
    _damageHandler.reset();
}