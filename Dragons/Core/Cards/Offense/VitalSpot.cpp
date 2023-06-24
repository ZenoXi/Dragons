#include "VitalSpot.h"

#include "../../Core.h"

cards::PlayResult cards::VitalSpot::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return PlayResult::AddToActives();
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

        auto cardPtr = core->RemoveCardFromActiveCards(this, GetPosition().playerIndex);
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

void cards::VitalSpot::_OnEnterDestroyedCards(Core* core)
{
    _preDamageHandler.reset();
}