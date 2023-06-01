#include "FairFight.h"

#include "../../Core.h"

cards::PlayResult cards::FairFight::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    // Give opponent 4 armor
    core->AddArmor(actionProps.opponent, 4);

    // Move card to active
    auto cardPtr = core->RemoveCardFromHand(this, actionProps.player);
    core->AddCardToActiveCards(std::move(cardPtr), actionProps.player);

    // Deal 4 damage at the end of your turn
    _turnEndHandler = std::make_unique<EventHandler<TurnEndEvent>>(core->Events(), [=](TurnEndEvent event)
    {
        if (event.playerIndex != actionProps.player)
            return;

        DamageProperties damageProps;
        damageProps.target = actionProps.opponent;
        damageProps.amount = 4;
        core->Damage(damageProps);

        // Move card to graveyard
        auto cardPtr = core->RemoveCardFromActiveCards(this, actionProps.player);
        core->AddCardToGraveyard(std::move(cardPtr));

        // Disable further handling
        _turnEndHandler->SetHandler([](TurnEndEvent) {});
    });

    // Do not discard
    PlayResult result{};
    result.discard = false;
    return result;
}