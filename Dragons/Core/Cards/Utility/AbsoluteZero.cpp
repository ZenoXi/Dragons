#include "AbsoluteZero.h"

#include "../../Core.h"

cards::PlayResult cards::AbsoluteZero::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return PlayResult::AddToActives();
}

void cards::AbsoluteZero::_OnEnterHand(Core* core, int playerIndex)
{
    _turnBeginHandler.reset();
}

void cards::AbsoluteZero::_OnEnterActiveCards(Core* core, int playerIndex)
{
    if (_turnBeginHandler)
        return;

    _turnBeginHandler = std::make_unique<EventHandler<TurnBeginEvent>>(&core->Events(), [=](TurnBeginEvent event)
    {
        if (event.opponentIndex != GetPosition().playerIndex)
            return;

        core->EndTurn();

        core->AddCardToGraveyard(core->RemoveCardFromActiveCards(this, GetPosition().playerIndex));
    });
}

void cards::AbsoluteZero::_OnEnterDeck(Core* core)
{
    _turnBeginHandler.reset();
}

void cards::AbsoluteZero::_OnEnterGraveyard(Core* core)
{
    _turnBeginHandler.reset();
}

void cards::AbsoluteZero::_OnEnterDestroyedCards(Core* core)
{
    _turnBeginHandler.reset();
}