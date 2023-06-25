#include "HellfireSword.h"

#include "../../Core.h"

cards::PlayResult cards::HellfireSword::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto playPropsValue = GetPlayProperties<HellfireSwordPlayProperties>(playProps);

    if (playPropsValue.stealArmor)
    {
        int opponentArmorValue = core->GetState().players[actionProps.opponent].armor;
        core->SetArmor(actionProps.opponent, 0, true);

        AddArmorProperties addArmorProps;
        addArmorProps.target = actionProps.player;
        addArmorProps.amount = opponentArmorValue;
        addArmorProps.sourceCard = this;
        core->AddArmor(addArmorProps);
    }
    else
    {
        core->DestroyArmor(actionProps.opponent);
    }
    return PlayResult::Default();
}