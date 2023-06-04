#include "ArmorUp.h"

#include "../../Core.h"

cards::PlayResult cards::ArmorUp::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    core->AddArmor(actionProps.player, 2);
    return PlayResult::Default();
}