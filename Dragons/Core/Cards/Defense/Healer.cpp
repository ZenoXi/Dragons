#include "Healer.h"

#include "../../Core.h"

cards::PlayResult cards::Healer::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto playPropsValue = GetPlayProperties<HealerPlayProperties>(playProps);

    HealProperties healProps;
    healProps.target = actionProps.player;
    healProps.amount = playPropsValue.healAmount;
    healProps.sourceCard = this;
    core->Heal(healProps);
    return PlayResult::Default();
}