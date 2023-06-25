#include "Cure.h"

#include "../../Core.h"

cards::PlayResult cards::Cure::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    int curHealth = core->GetState().players[actionProps.player].health;
    int maxHealth = core->GetState().players[actionProps.player].maxHealth;
    HealProperties healProps;
    healProps.target = actionProps.player;
    healProps.amount = (maxHealth - curHealth) / 2;
    healProps.sourceCard = this;
    core->Heal(healProps);
    return PlayResult::Default();
}