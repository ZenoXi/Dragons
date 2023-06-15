#include "SummonDead.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::SummonDead::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return !core->GetState().graveyard.empty() && core->GetState().players[actionProps.player].hand.size() < GAME_HAND_SIZE;
}

cards::PlayResult cards::SummonDead::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (core->GetState().graveyard.empty() || core->GetState().players[actionProps.player].hand.size() >= GAME_HAND_SIZE)
        return PlayResult::Default();

    auto params = std::make_unique<UserInputParams_ChooseCardFromGraveyard>();
    params->playerIndex = actionProps.player;
    params->minCardCount = 1;
    params->maxCardCount = 1;

    _waitingForCardChoice = true;

    PlayResult result;
    result.waitForInput = true;
    result.inputRequest.inputType = UserInputType::CHOOSE_CARD_FROM_GRAVEYARD;
    result.inputRequest.inputPrompt = L"Select card";
    result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
    return result;
}

cards::PlayResult cards::SummonDead::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_waitingForCardChoice)
    {
        _waitingForCardChoice = false;

        UserInputParams_ChooseCardFromGraveyard* responseParams = reinterpret_cast<UserInputParams_ChooseCardFromGraveyard*>(response.inputParams.get());
        if (!responseParams)
            return PlayResult::Default();
        if (responseParams->chosenCards.size() != 1)
            return PlayResult::Default();

        Card* chosenCard = responseParams->chosenCards[0];
        auto cardPtr = core->RemoveCardFromGraveyard(chosenCard);
        core->AddCardToHand(std::move(cardPtr), actionProps.player);

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}