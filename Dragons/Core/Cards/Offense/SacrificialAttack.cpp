#include "SacrificialAttack.h"

#include "../../Core.h"

bool cards::SacrificialAttack::CanPlay(Core* core, ActionProperties actionProps)
{
    const GameState& state = core->GetState();
    return state.players[actionProps.player].hand.size() < state.players[actionProps.opponent].hand.size();
}

cards::PlayResult cards::SacrificialAttack::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    DamageProperties damageProps;
    damageProps.target = actionProps.opponent;
    damageProps.amount = 3;
    damageProps.ignoreArmor = false;
    damageProps.trueDamage = false;
    core->Damage(damageProps);

    damageProps.target = actionProps.player;
    core->Damage(damageProps);

    return PlayResult{};
}