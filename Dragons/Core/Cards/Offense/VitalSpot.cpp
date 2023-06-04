#include "VitalSpot.h"

#include "../../Core.h"

cards::PlayResult cards::VitalSpot::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    // Move card to active
    auto cardPtr = core->RemoveCardFromHand(this, actionProps.player);
    core->AddCardToActiveCards(std::move(cardPtr), actionProps.player);

    return PlayResult::DontDiscard();
}

void cards::VitalSpot::_OnEnterHand(Core* core, int playerIndex)
{
    _preDamageHandler.reset();
}

void cards::VitalSpot::_OnEnterActiveCards(Core* core, int playerIndex)
{
    if (_preDamageHandler)
        return;

    _preDamageHandler = std::make_unique<EventHandler<PreDamageEvent_BuffPass>>(&core->Events(), [=](PreDamageEvent_BuffPass event)
    {
        if (GetPosition().playerIndex != event.props->source)
            return;

        if (event.props->trueDamage && event.props->ignoreArmor)
            return;
        event.props->trueDamage = true;
        event.props->ignoreArmor = true;

        auto cardPtr = core->RemoveCardFromActiveCards(this, event.props->source);
        core->AddCardToGraveyard(std::move(cardPtr));
    });
}

void cards::VitalSpot::_OnEnterDeck(Core* core)
{
    _preDamageHandler.reset();
}

void cards::VitalSpot::_OnEnterGraveyard(Core* core)
{
    _preDamageHandler.reset();
}