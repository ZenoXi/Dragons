#include "ShieldingNet.h"

#include "../../Core.h"

cards::PlayResult cards::ShieldingNet::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    // Move card to active
    auto cardPtr = core->RemoveCardFromHand(this, actionProps.player);
    core->AddCardToActiveCards(std::move(cardPtr), actionProps.player);

    return PlayResult::DontDiscard();
}

void cards::ShieldingNet::_OnEnterHand(Core* core, int playerIndex)
{
    _damageHandler.reset();
}

void cards::ShieldingNet::_OnEnterActiveCards(Core* core, int playerIndex)
{
    _damageHandler = std::make_unique<EventHandler<PreDamageEvent_NerfPass>>(&core->Events(), [=](PreDamageEvent_NerfPass event)
    {
        if (event.props->target != GetPosition().playerIndex)
            return;

        if (event.props->trueDamage)
            return;
        if (event.props->amount > 1)
            event.props->amount--;
    });
}

void cards::ShieldingNet::_OnEnterDeck(Core* core)
{
    _damageHandler.reset();
}

void cards::ShieldingNet::_OnEnterGraveyard(Core* core)
{
    _damageHandler.reset();
}

void cards::ShieldingNet::_OnEnterDestroyedCards(Core* core)
{
    _damageHandler.reset();
}