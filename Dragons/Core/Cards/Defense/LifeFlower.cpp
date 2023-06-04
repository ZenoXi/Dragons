#include "LifeFlower.h"

#include "../../Core.h"

cards::PlayResult cards::LifeFlower::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    core->SetMaxHealth(actionProps.player, core->GetState().players[actionProps.player].maxHealth + 1);
    core->Heal(actionProps.player, 1);
    core->AddArmor(actionProps.player, 1);
    return PlayResult::Default();
}