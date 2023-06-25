#include "TreeOfLife.h"

#include "../../Core.h"

cards::PlayResult cards::TreeOfLife::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto playPropsValue = GetPlayProperties<TreeOfLifePlayProperties>(playProps);

    HealProperties healProps;
    healProps.target = actionProps.player;
    healProps.amount = 4;
    healProps.sourceCard = this;
    core->Heal(healProps);
    if (playPropsValue.healOpponent)
    {
        healProps.target = actionProps.opponent;
        core->Heal(healProps);
    }
    return PlayResult::Default();
}