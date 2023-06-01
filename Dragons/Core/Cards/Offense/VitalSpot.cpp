#include "VitalSpot.h"

#include "../../Core.h"

cards::PlayResult cards::VitalSpot::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    // Move card to active
    auto cardPtr = core->RemoveCardFromHand(this, actionProps.player);
    core->AddCardToActiveCards(std::move(cardPtr), actionProps.player);

    // Deal 4 damage at the end of your turn
    _preDamageHandler = std::make_unique<EventHandler<PreDamageEvent>>(core->Events(), [=](PreDamageEvent event)
    {
        if (event.props->source == -1)
            return;

        auto& sourcePlayerActives = core->GetState().players[event.props->source].activeCards;
        auto it = std::find_if(sourcePlayerActives.begin(), sourcePlayerActives.end(), [=](const std::unique_ptr<cards::Card>& card) { return card.get() == this; });
        if (it == sourcePlayerActives.end())
            return;

        if (event.props->trueDamage && event.props->ignoreArmor)
            return;

        event.props->trueDamage = true;
        event.props->ignoreArmor = true;

        // Move card to graveyard
        auto cardPtr = core->RemoveCardFromActiveCards(this, event.props->source);
        core->AddCardToGraveyard(std::move(cardPtr));

        // Disable further handling
        _preDamageHandler->SetHandler([](PreDamageEvent) {});
    });

    // Do not discard
    PlayResult result{};
    result.discard = false;
    return result;
}