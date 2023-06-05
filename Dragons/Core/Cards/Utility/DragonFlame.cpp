#include "DragonFlame.h"

#include "../../Core.h"

bool cards::DragonFlame::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return !core->GetState().players[actionProps.opponent].activeCards.empty();
}

cards::PlayResult cards::DragonFlame::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    while (!core->GetState().players[actionProps.opponent].activeCards.empty())
    {
        auto cardPtr = core->RemoveCardFromActiveCards(core->GetState().players[actionProps.opponent].activeCards.front().get(), actionProps.opponent);
        core->AddCardToDestroyedCards(std::move(cardPtr));
    }
    return PlayResult::Default();
}