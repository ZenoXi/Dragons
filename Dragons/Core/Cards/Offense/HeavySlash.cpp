#include "HeavySlash.h"

#include "../../Core.h"

cards::PlayResult cards::HeavySlash::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    _playerIndex = actionProps.player;
    _opponentIndex = actionProps.opponent;

    return PlayResult::AddToActives();
}

void cards::HeavySlash::_OnEnterHand(Core* core, int playerIndex)
{
    _turnBeginHandler.reset();
}

void cards::HeavySlash::_OnEnterActiveCards(Core* core, int playerIndex)
{
    if (_turnBeginHandler)
        return;

    _turnBeginHandler = std::make_unique<EventHandler<TurnBeginEvent>>(&core->Events(), [=](TurnBeginEvent event)
    {
        if (event.playerIndex != _playerIndex)
            return;

        DamageProperties damageProps;
        damageProps.target = _opponentIndex;
        if (core->GetState().players[_opponentIndex].armor > 0)
            damageProps.amount = 1;
        else
            damageProps.amount = 3;
        core->Damage(damageProps);

        auto cardPtr = core->RemoveCardFromActiveCards(this, _playerIndex);
        core->AddCardToGraveyard(std::move(cardPtr));
    });
}

void cards::HeavySlash::_OnEnterDeck(Core* core)
{
    _turnBeginHandler.reset();
}

void cards::HeavySlash::_OnEnterGraveyard(Core* core)
{
    _turnBeginHandler.reset();
}

void cards::HeavySlash::_OnEnterDestroyedCards(Core* core)
{
    _turnBeginHandler.reset();
}