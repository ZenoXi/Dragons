#include "Stab.h"

#include "../../Core.h"

cards::PlayResult cards::Stab::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    DamageProperties damageProps;
    damageProps.source = actionProps.player;
    damageProps.target = actionProps.opponent;
    damageProps.amount = 2;
    damageProps.sourceCard = this;
    core->Damage(damageProps);

    return PlayResult::Default();
}