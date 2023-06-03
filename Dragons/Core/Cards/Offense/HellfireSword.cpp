#include "HellfireSword.h"

#include "../../Core.h"

cards::PlayResult cards::HellfireSword::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    core->DestroyArmor(actionProps.opponent);
    return PlayResult::Default();
}