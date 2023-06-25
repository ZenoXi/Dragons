#include "FairFight.h"

#include "../../Core.h"

cards::PlayResult cards::FairFight::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto playPropsValue = GetPlayProperties<FairFightPlayProperties>(playProps);

    if (playPropsValue.giveOpponentArmor)
    {
        AddArmorProperties addArmorProps;
        addArmorProps.target = actionProps.player;
        addArmorProps.amount = 4;
        addArmorProps.sourceCard = this;
        core->AddArmor(addArmorProps);
    }

    return PlayResult::AddToActives();
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