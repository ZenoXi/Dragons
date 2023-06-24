#include "Barrier.h"

#include "../../Core.h"

cards::PlayResult cards::Barrier::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return PlayResult::AddToActives();
}

void cards::Barrier::_OnEnterHand(Core* core, int playerIndex)
{
    _preDamageHandler.reset();
}

void cards::Barrier::_OnEnterActiveCards(Core* core, int playerIndex)
{
    if (_preDamageHandler)
        return;

    _preDamageHandler = std::make_unique<EventHandler<PreDamageEvent_NerfPass>>(&core->Events(), [=](PreDamageEvent_NerfPass event)
    {
        if (GetPosition().playerIndex != event.props->target)
            return;
        if (event.props->trueDamage || event.props->fatigue)
            return;

        event.props->amount = 0;

        auto cardPtr = core->RemoveCardFromActiveCards(this, GetPosition().playerIndex);
        core->AddCardToGraveyard(std::move(cardPtr));
    });
}

void cards::Barrier::_OnEnterDeck(Core* core)
{
    _preDamageHandler.reset();
}

void cards::Barrier::_OnEnterGraveyard(Core* core)
{
    _preDamageHandler.reset();
}

void cards::Barrier::_OnEnterDestroyedCards(Core* core)
{
    _preDamageHandler.reset();
}