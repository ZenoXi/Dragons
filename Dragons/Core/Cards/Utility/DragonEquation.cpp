#include "DragonEquation.h"

#include "../../Core.h"

bool cards::DragonEquation::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return !core->GetState().players[actionProps.player].activeCards.empty() || !core->GetState().players[actionProps.opponent].activeCards.empty();
}

cards::PlayResult cards::DragonEquation::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (core->GetState().players[actionProps.player].activeCards.empty() && core->GetState().players[actionProps.opponent].activeCards.empty())
        return PlayResult::Default();

    std::vector<std::unique_ptr<Card>> temp;
    while (!core->GetState().players[actionProps.player].activeCards.empty())
    {
        temp.push_back(core->RemoveCardFromActiveCards(core->GetState().players[actionProps.player].activeCards.front().get(), actionProps.player));
    }

    while (!core->GetState().players[actionProps.opponent].activeCards.empty())
    {
        auto cardPtr = core->RemoveCardFromActiveCards(core->GetState().players[actionProps.opponent].activeCards.front().get(), actionProps.opponent);
        core->AddCardToActiveCards(std::move(cardPtr), actionProps.player);
    }

    while (!temp.empty())
    {
        core->AddCardToActiveCards(std::move(temp.front()), actionProps.opponent);
        temp.erase(temp.begin());
    }

    return PlayResult::Default();
}