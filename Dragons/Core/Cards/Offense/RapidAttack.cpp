#include "RapidAttack.h"

#include "../../Core.h"

bool cards::RapidAttack::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return !_playedThisTurn || _playedBy != actionProps.player;
}

cards::PlayResult cards::RapidAttack::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (!CanPlay(core, actionProps, playProps))
    {
        PlayResult result{};
        result.discard = false;
        result.notPlayed = true;
        return result;
    }

    // Deal 1 damage
    DamageProperties damageProps;
    damageProps.source = actionProps.player;
    damageProps.target = actionProps.opponent;
    damageProps.amount = 1;
    core->Damage(damageProps);

    _playedThisTurn = true;
    _playedBy = actionProps.player;

    // At the end of the turn reset play flag
    _turnEndHandler = std::make_unique<EventHandler<TurnEndEvent>>(&core->Events(), [=](TurnEndEvent event)
    {
        _playedThisTurn = false;
    });

    return PlayResult::DontDiscard();
}