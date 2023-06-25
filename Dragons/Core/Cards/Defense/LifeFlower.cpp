#include "LifeFlower.h"

#include "../../Core.h"

cards::PlayResult cards::LifeFlower::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    core->SetMaxHealth(actionProps.player, core->GetState().players[actionProps.player].maxHealth + 1);
    HealProperties healProps;
    healProps.target = actionProps.player;
    healProps.amount = 1;
    healProps.sourceCard = this;
    core->Heal(healProps);
    AddArmorProperties addArmorProps;
    addArmorProps.target = actionProps.player;
    addArmorProps.amount = 1;
    addArmorProps.sourceCard = this;
    core->AddArmor(addArmorProps);
    return PlayResult::Default();
}