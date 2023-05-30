#include "DesperateStrike.h"

#include "../../Core.h"

bool cards::DesperateStrike::CanPlay(Core* core, ActionProperties actionProps)
{
    const GameState& state = core->GetState();
    return state.players[actionProps.player].hand.size() < state.players[actionProps.opponent].hand.size();
}

cards::PlayResult cards::DesperateStrike::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    const GameState& state = core->GetState();
    if (state.players[actionProps.player].hand.size() < state.players[actionProps.opponent].hand.size())
    {
        DamageProperties damageProps;
        damageProps.target = actionProps.opponent;
        damageProps.amount = 3;
        damageProps.ignoreArmor = false;
        damageProps.trueDamage = false;
        core->Damage(damageProps);
    }
    return PlayResult{};
}