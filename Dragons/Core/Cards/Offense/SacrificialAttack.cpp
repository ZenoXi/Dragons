#include "SacrificialAttack.h"

#include "../../Core.h"

cards::PlayResult cards::SacrificialAttack::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto playPropsValue = GetPlayProperties<SacrificialAttackPlayProperties>(playProps);

    int removedHealthFromSelf = 0;
    int removedHealthFromOpponent = 0;
    for (auto& player : core->GetState().players)
    {
        if (!playPropsValue.damageSelf && player.index == actionProps.player)
            continue;

        DamageProperties damageProps;
        damageProps.source = actionProps.player;
        damageProps.sourceCard = this;
        damageProps.amount = 3;

        damageProps.target = player.index;
        if (playPropsValue.stealHealth && player.index == actionProps.opponent)
            damageProps.ignoreArmor = true;
        DamageResult result = core->Damage(damageProps);

        if (player.index == actionProps.player)
            removedHealthFromSelf = result.removedHealthAmount;
        else if (player.index == actionProps.opponent)
            removedHealthFromOpponent = result.removedHealthAmount;
    }

    if (playPropsValue.stealHealth && removedHealthFromOpponent > 0)
    {
        HealProperties healProps;
        healProps.target = actionProps.player;
        healProps.amount = removedHealthFromOpponent;
        healProps.sourceCard = this;
        core->Heal(healProps);
    }

    return PlayResult::Default();
}