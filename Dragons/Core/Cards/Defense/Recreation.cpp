#include "Recreation.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::Recreation::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->GetState().defenseDeck.empty();
}

cards::PlayResult cards::Recreation::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto& graveyardRef = core->GetState().graveyard;
    for (int i = 0; i < graveyardRef.size(); i++)
    {
        if (graveyardRef[i]->GetCardType() == CardType::DEFENSE)
        {
            auto cardPtr = core->RemoveCardFromGraveyard(graveyardRef[i].get());
            core->AddCardToDeck(std::move(cardPtr));
            i--;
        }
    }
    core->ShuffleDeck(CardType::DEFENSE);

    if (core->GetState().players[actionProps.player].hand.size() < GAME_HAND_SIZE)
        core->DrawCard(CardType::DEFENSE, actionProps.player, false);
    if (core->GetState().players[actionProps.player].hand.size() < GAME_HAND_SIZE)
        core->DrawCard(CardType::DEFENSE, actionProps.player, false);

    return PlayResult::Default();
}