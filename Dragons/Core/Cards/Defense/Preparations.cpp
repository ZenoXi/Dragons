#include "Preparations.h"

#include "../../Core.h"

cards::PlayResult cards::Preparations::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    int offenseCards = core->GetState().players[actionProps.opponent].CardsInHand(CardType::OFFENSE);
    if (offenseCards > 0)
        core->AddArmor(actionProps.player, offenseCards);

    return PlayResult::Default();
}