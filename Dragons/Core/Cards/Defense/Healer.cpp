#include "Healer.h"

#include "../../Core.h"

cards::PlayResult cards::Healer::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    core->Heal(actionProps.player, 2);
    return PlayResult::Default();
}