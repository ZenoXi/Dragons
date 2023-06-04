#include "WarBlessing.h"

#include "../../Core.h"

cards::PlayResult cards::WarBlessing::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    core->SetMaxHealth(actionProps.player, core->GetState().players[actionProps.player].maxHealth + 5);
    core->Heal(actionProps.player, 1);
    return PlayResult::Default();
}