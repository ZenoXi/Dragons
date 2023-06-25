#include "ArmorUp.h"

#include "../../Core.h"

cards::PlayResult cards::ArmorUp::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto playPropsValue = GetPlayProperties<ArmorUpPlayProperties>(playProps);

    AddArmorProperties addArmorProps;
    addArmorProps.target = actionProps.player;
    addArmorProps.amount = playPropsValue.armorAmount;
    addArmorProps.sourceCard = this;
    core->AddArmor(addArmorProps);
    return PlayResult::Default();
}