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

    // Request user input
    auto params = std::make_unique<UserInputParams_ChooseCardFromHand>();
    params->choosingPlayerIndex = actionProps.player;
    params->handPlayerIndex = actionProps.opponent;
    params->minCardCount = 1;
    params->maxCardCount = 1;

    _waitingForCardChoice = true;

    PlayResult result;
    result.waitForInput = true;
    result.inputRequest.inputType = UserInputType::CHOOSE_CARD_FROM_HAND;
    result.inputRequest.inputPrompt = L"Choose a card to take from your opponent";
    result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
    return result;
}

cards::PlayResult cards::TheThief::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_waitingForCardChoice)
    {
        _waitingForCardChoice = false;

        UserInputParams_ChooseCardFromHand* params = reinterpret_cast<UserInputParams_ChooseCardFromHand*>(response.inputParams.get());
        Card* chosenCard = params->chosenCards[0];

        auto cardPtr = core->RemoveCardFromHand(chosenCard, actionProps.opponent);
        core->AddCardToHand(std::move(cardPtr), actionProps.player);

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}