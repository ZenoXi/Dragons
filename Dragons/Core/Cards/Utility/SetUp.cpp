#include "SetUp.h"

#include "../../Core.h"

cards::PlayResult cards::SetUp::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    // Move card to active
    auto cardPtr = core->RemoveCardFromHand(this, actionProps.player);
    core->AddCardToActiveCards(std::move(cardPtr), actionProps.player);

    return PlayResult::DontDiscard();
}

void cards::SetUp::_OnEnterHand(Core* core, int playerIndex)
{
    _turnBeginHandler.reset();
}

void cards::SetUp::_OnEnterActiveCards(Core* core, int playerIndex)
{
    if (_turnBeginHandler)
        return;

    _turnBeginHandler = std::make_unique<EventHandler<TurnBeginEvent>>(&core->Events(), [=](TurnBeginEvent event)
    {
        if (event.playerIndex != GetPosition().playerIndex)
            return;

        core->SetActionCount(event.playerIndex, 4);
    });
}

void cards::SetUp::_OnEnterDeck(Core* core)
{
    _turnBeginHandler.reset();
}

void cards::SetUp::_OnEnterGraveyard(Core* core)
{
    _turnBeginHandler.reset();
}

void cards::SetUp::_OnEnterDestroyedCards(Core* core)
{
    _turnBeginHandler.reset();
}