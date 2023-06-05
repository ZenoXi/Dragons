#include "FairFight.h"

#include "../../Core.h"

cards::PlayResult cards::FairFight::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    // Give opponent 4 armor
    core->AddArmor(actionProps.opponent, 4);

    // Move card to active
    auto cardPtr = core->RemoveCardFromHand(this, actionProps.player);
    core->AddCardToActiveCards(std::move(cardPtr), actionProps.player);

    return PlayResult::DontDiscard();
}

void cards::FairFight::_OnEnterHand(Core* core, int playerIndex)
{
    _turnEndHandler.reset();
}

void cards::FairFight::_OnEnterActiveCards(Core* core, int playerIndex)
{
    if (_turnEndHandler)
        return;

    _turnEndHandler = std::make_unique<EventHandler<TurnEndEvent>>(&core->Events(), [=](TurnEndEvent event)
    {
        if (event.playerIndex != GetPosition().playerIndex)
            return;

        DamageProperties damageProps;
        damageProps.target = event.opponentIndex;
        damageProps.amount = 4;
        core->Damage(damageProps);

        auto cardPtr = core->RemoveCardFromActiveCards(this, event.playerIndex);
        core->AddCardToGraveyard(std::move(cardPtr));
    });
}

void cards::FairFight::_OnEnterDeck(Core* core)
{
    _turnEndHandler.reset();
}

void cards::FairFight::_OnEnterGraveyard(Core* core)
{
    _turnEndHandler.reset();
}

void cards::FairFight::_OnEnterDestroyedCards(Core* core)
{
    _turnEndHandler.reset();
}