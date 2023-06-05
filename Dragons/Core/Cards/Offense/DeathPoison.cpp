#include "DeathPoison.h"

#include "../../Core.h"

cards::PlayResult cards::DeathPoison::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    // Move card to active
    auto cardPtr = core->RemoveCardFromHand(this, actionProps.player);
    core->AddCardToActiveCards(std::move(cardPtr), actionProps.player);

    return PlayResult::DontDiscard();
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