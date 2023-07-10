#include "DesperateStrike.h"

#include "../../Core.h"

bool cards::DesperateStrike::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    const GameState& state = core->GetState();
    return state.players[actionProps.player].hand.size() < state.players[actionProps.opponent].hand.size();
}

cards::PlayResult cards::DesperateStrike::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (!CanPlay(core, actionProps, playProps))
        return PlayResult::Default();

    auto playPropsValue = GetPlayProperties<DesperateStrikePlayProperties>(playProps);

    const GameState& state = core->GetState();
    if (state.players[actionProps.player].hand.size() < state.players[actionProps.opponent].hand.size())
    {
        DamageProperties damageProps;
        damageProps.source = actionProps.player;
        damageProps.target = actionProps.opponent;
        damageProps.sourceCard = this;
        damageProps.amount = playPropsValue.damageAmount;
        core->Damage(damageProps);
    }
    return PlayResult::Default();
}