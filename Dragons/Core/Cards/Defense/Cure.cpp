#include "Cure.h"

#include "../../Core.h"

cards::PlayResult cards::Cure::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    int curHealth = core->GetState().players[actionProps.player].health;
    int maxHealth = core->GetState().players[actionProps.player].maxHealth;
    core->Heal(actionProps.player, (maxHealth - curHealth) / 2);
    return PlayResult::Default();
}