#include "LifeFlower.h"

#include "../../Core.h"

cards::PlayResult cards::LifeFlower::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto playPropsValue = GetPlayProperties<LifeFlowerPlayProperties>(playProps);

    core->SetMaxHealth(actionProps.player, core->GetState().players[actionProps.player].maxHealth + playPropsValue.maxHealthIncrease);
    HealProperties healProps;
    healProps.target = actionProps.player;
    healProps.amount = playPropsValue.healAmount;
    healProps.sourceCard = this;
    core->Heal(healProps);
    AddArmorProperties addArmorProps;
    addArmorProps.target = actionProps.player;
    addArmorProps.amount = playPropsValue.armorAmount;
    addArmorProps.sourceCard = this;
    core->AddArmor(addArmorProps);
    return PlayResult::Default();
}