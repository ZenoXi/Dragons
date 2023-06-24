#include "FireMoon.h"

#include "../../Core.h"

cards::PlayResult cards::FireMoon::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    // Request user input
    auto params = std::make_unique<UserInputParams_DrawCard>();
    params->minCardCount = 1;
    params->maxCardCount = 1;
    params->playerIndex = actionProps.player;

    _waitingForCardDraw = true;

    PlayResult result;
    result.waitForInput = true;
    result.inputRequest.inputType = UserInputType::DRAW_CARD;
    result.inputRequest.inputPrompt = L"Draw a card";
    result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
    return result;
}

cards::PlayResult cards::FireMoon::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_waitingForCardDraw)
    {
        _waitingForCardDraw = false;
        return PlayResult::AddToActives();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}

void cards::FireMoon::_OnEnterHand(Core* core, int playerIndex)
{
    _turnBeginHandler.reset();
    _turnEndHandler.reset();
    _canPlayHandler.reset();
    _activated = false;
}

void cards::FireMoon::_OnEnterActiveCards(Core* core, int playerIndex)
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
    _canPlayHandler = std::make_unique<EventHandler<CanPlayEvent>>(&core->Events(), [=](CanPlayEvent event)
    {
        if (!_activated)
            return;
        if (event.actionProps->opponent != GetPosition().playerIndex)
            return;

        if (event.card->GetCardType() == CardType::DEFENSE)
            *event.canPlay = false;
    });
}

void cards::FireMoon::_OnEnterDeck(Core* core)
{
    _turnBeginHandler.reset();
    _turnEndHandler.reset();
    _canPlayHandler.reset();
    _activated = false;
}

void cards::FireMoon::_OnEnterGraveyard(Core* core)
{
    _turnBeginHandler.reset();
    _turnEndHandler.reset();
    _canPlayHandler.reset();
    _activated = false;
}

void cards::FireMoon::_OnEnterDestroyedCards(Core* core)
{
    _turnBeginHandler.reset();
    _turnEndHandler.reset();
    _canPlayHandler.reset();
    _activated = false;
}