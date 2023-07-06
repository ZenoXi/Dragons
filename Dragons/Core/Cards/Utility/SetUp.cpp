#include "SetUp.h"

#include "../../Core.h"

cards::PlayResult cards::SetUp::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto playPropsValue = GetPlayProperties<SetUpPlayProperties>(playProps);
    _actionCount = playPropsValue.actionCount;

    return PlayResult::AddToActives();
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

        core->SetActionCount(event.playerIndex, _actionCount);

        core->AddCardToGraveyard(core->RemoveCardFromActiveCards(this, GetPosition().playerIndex));
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