#include "SacrificialAttack.h"

#include "../../Core.h"

cards::PlayResult cards::SacrificialAttack::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto playPropsValue = GetPlayProperties<SacrificialAttackPlayProperties>(playProps);

    DamageProperties damageProps;
    damageProps.source = actionProps.player;
    damageProps.target = actionProps.opponent;
    damageProps.amount = 3;
    damageProps.sourceCard = this;
    if (playPropsValue.stealHealth)
        damageProps.ignoreArmor = true;
    DamageResult result = core->Damage(damageProps);

    if (playPropsValue.damageSelf)
    {
        damageProps.source = -1;
        damageProps.target = actionProps.player;
        core->Damage(damageProps);
    }

    if (playPropsValue.stealHealth && result.removedHealthAmount > 0)
    {
        HealProperties healProps;
        healProps.target = actionProps.player;
        healProps.amount = result.removedHealthAmount;
        healProps.sourceCard = this;
        core->Heal(healProps);
    }

    return PlayResult::Default();
}