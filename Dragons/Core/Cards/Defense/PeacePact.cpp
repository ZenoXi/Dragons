#include "PeacePact.h"

#include "../../Core.h"

cards::PlayResult cards::PeacePact::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto playPropsValue = GetPlayProperties<PeacePactPlayProperties>(playProps);

    for (auto& player : core->GetState().players)
    {
        if (!playPropsValue.discardSelf && player.index == actionProps.player)
            continue;

        for (int i = 0; i < player.hand.size(); i++)
        {
            if (player.hand[i]->GetCardType() == CardType::OFFENSE)
            {
                auto cardPtr = core->RemoveCardFromHand(player.hand[i].get(), player.index);
                core->AddCardToDeck(std::move(cardPtr));
            }
            i--;
        }
    }
    core->ShuffleDeck(CardType::OFFENSE);

    return PlayResult::Default();
}