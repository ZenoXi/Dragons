#include "TreeOfLife.h"

#include "../../Core.h"

cards::PlayResult cards::TreeOfLife::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    core->Heal(actionProps.player, 4);
    core->Heal(actionProps.opponent, 4);
    return PlayResult::Default();
}