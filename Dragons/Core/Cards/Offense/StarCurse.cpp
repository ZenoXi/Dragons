#include "StarCurse.h"

#include "../../Core.h"

cards::PlayResult cards::StarCurse::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    core->SetMaxHealth(actionProps.opponent, core->GetState().players[actionProps.opponent].health);
    return PlayResult{};
}