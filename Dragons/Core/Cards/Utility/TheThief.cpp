#include "TheThief.h"

#include "../../Core.h"

bool cards::TheThief::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return !core->GetState().players[actionProps.opponent].hand.empty();
}

cards::PlayResult cards::TheThief::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (core->GetState().players[actionProps.opponent].hand.empty())
        return PlayResult::Default();

    int cardIndex = core->GenerateRandomNumber(0, core->GetState().players[actionProps.opponent].hand.size());
    auto cardPtr = core->RemoveCardFromHand(cardIndex, actionProps.opponent);
    core->AddCardToHand(std::move(cardPtr), actionProps.player);

    return PlayResult::Default();
}