#include "WarBlessing.h"

#include "../../Core.h"

cards::PlayResult cards::WarBlessing::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    core->SetMaxHealth(actionProps.player, core->GetState().players[actionProps.player].maxHealth + 5);
    HealProperties healProps;
    healProps.target = actionProps.player;
    healProps.amount = 1;
    healProps.sourceCard = this;
    core->Heal(healProps);
    return PlayResult::Default();
}