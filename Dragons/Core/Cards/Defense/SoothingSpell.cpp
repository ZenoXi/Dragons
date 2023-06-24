#include "SoothingSpell.h"

#include "../../Core.h"

cards::PlayResult cards::SoothingSpell::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    core->AddExtraPlays(actionProps.player, 1);

    return PlayResult::AddToActives();
}

void cards::SoothingSpell::_OnEnterHand(Core* core, int playerIndex)
{
    _turnBeginHandler.reset();
    _turnEndHandler.reset();
    _canPlayHandler.reset();
    _activated = false;
}

void cards::SoothingSpell::_OnEnterActiveCards(Core* core, int playerIndex)
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

        if (event.card->GetCardType() == CardType::OFFENSE)
            *event.canPlay = false;
    });
}

void cards::SoothingSpell::_OnEnterDeck(Core* core)
{
    _turnBeginHandler.reset();
    _turnEndHandler.reset();
    _canPlayHandler.reset();
    _activated = false;
}

void cards::SoothingSpell::_OnEnterGraveyard(Core* core)
{
    _turnBeginHandler.reset();
    _turnEndHandler.reset();
    _canPlayHandler.reset();
    _activated = false;
}

void cards::SoothingSpell::_OnEnterDestroyedCards(Core* core)
{
    _turnBeginHandler.reset();
    _turnEndHandler.reset();
    _canPlayHandler.reset();
    _activated = false;
}