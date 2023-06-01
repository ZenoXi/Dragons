#include "SacrificialAttack.h"

#include "../../Core.h"

cards::PlayResult cards::SacrificialAttack::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    DamageProperties damageProps;
    damageProps.source = actionProps.player;
    damageProps.target = actionProps.opponent;
    damageProps.amount = 3;
    core->Damage(damageProps);

    damageProps.source = -1;
    damageProps.target = actionProps.player;
    core->Damage(damageProps);

    return PlayResult{};
}