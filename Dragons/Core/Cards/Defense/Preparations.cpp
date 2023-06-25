#include "Preparations.h"

#include "../../Core.h"

cards::PlayResult cards::Preparations::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    int offenseCards = core->GetState().players[actionProps.opponent].CardsInHand(CardType::OFFENSE);
    if (offenseCards > 0)
    {
        AddArmorProperties addArmorProps;
        addArmorProps.target = actionProps.player;
        addArmorProps.amount = offenseCards;
        addArmorProps.sourceCard = this;
        core->AddArmor(addArmorProps);
    }

    return PlayResult::Default();
}