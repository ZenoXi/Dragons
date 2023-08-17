#include "FairFight.h"

#include "../../Core.h"

cards::PlayResult cards::FairFight::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (actionProps.opponent == actionProps.player)
        _opponent = _RelativeTarget::OWNER;

    auto playPropsValue = GetPlayProperties<FairFightPlayProperties>(playProps);

    if (playPropsValue.giveOpponentArmor)
    {
        AddArmorProperties addArmorProps;
        addArmorProps.target = _TargetPlayerIndex(_opponent);
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
        if (event.playerIndex != _TargetPlayerIndex(_player))
            return;

        DamageProperties damageProps;
        damageProps.source = _TargetPlayerIndex(_player);
        damageProps.target = _TargetPlayerIndex(_opponent);
        damageProps.sourceCard = this;
        damageProps.amount = 4;
        core->Damage(damageProps);

        auto cardPtr = core->RemoveCardFromActiveCards(this, GetPosition().playerIndex);
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