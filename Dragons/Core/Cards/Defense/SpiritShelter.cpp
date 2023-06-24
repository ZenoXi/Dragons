#include "SpiritShelter.h"

#include "../../Core.h"

cards::PlayResult cards::SpiritShelter::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return PlayResult::AddToActives();
}

void cards::SpiritShelter::_OnEnterHand(Core* core, int playerIndex)
{
    _damageHandler.reset();
}

void cards::SpiritShelter::_OnEnterActiveCards(Core* core, int playerIndex)
{
    _damageHandler = std::make_unique<EventHandler<PreDamageEvent_NerfPass>>(&core->Events(), [=](PreDamageEvent_NerfPass event)
    {
        if (event.props->target != GetPosition().playerIndex)
            return;

        if (event.props->fatigue)
            event.props->amount = 0;
    });
}

void cards::SpiritShelter::_OnEnterDeck(Core* core)
{
    _damageHandler.reset();
}

void cards::SpiritShelter::_OnEnterGraveyard(Core* core)
{
    _damageHandler.reset();
}

void cards::SpiritShelter::_OnEnterDestroyedCards(Core* core)
{
    _damageHandler.reset();
}