#include "DragonSight.h"

#include "../../Core.h"

cards::PlayResult cards::DragonSight::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    std::array<bool, 2> displayedTo;
    displayedTo[actionProps.player] = true;
    displayedTo[actionProps.opponent] = false;
    if (!core->GetState().offenseDeck.empty())
        core->AddCardToDisplayedCards({ core->GetState().offenseDeck.back().get(), displayedTo });
    if (!core->GetState().defenseDeck.empty())
        core->AddCardToDisplayedCards({ core->GetState().defenseDeck.back().get(), displayedTo });
    if (!core->GetState().utilityDeck.empty())
        core->AddCardToDisplayedCards({ core->GetState().utilityDeck.back().get(), displayedTo });
    if (!core->GetState().comboDeck.empty())
        core->AddCardToDisplayedCards({ core->GetState().comboDeck.back().get(), displayedTo });
    int cardsRevealed = core->GetState().displayedCards.size();
    
    if (cardsRevealed < 4)
    {
        auto params = std::make_unique<UserInputParams_ChooseDeck>();
        params->playerIndex = actionProps.player;
        params->minDeckCount = 1;
        params->maxDeckCount = 1;
        params->allowEmptyDecks = false;

        _waitingForDeckChoice = true;

        PlayResult result;
        result.waitForInput = true;
        result.inputRequest.inputType = UserInputType::CHOOSE_DECK;
        result.inputRequest.inputPrompt = L"Select deck to reveal a card";
        result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
        return result;
    }
    else
    {
        auto params = std::make_unique<UserInputParams_WaitForConfirmation>();
        params->playerIndex = actionProps.player;

        _waitingForConfirmation = true;

        PlayResult result;
        result.waitForInput = true;
        result.inputRequest.inputType = UserInputType::WAIT_FOR_CONFIRMATION;
        result.inputRequest.inputPrompt = L"Done";
        result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
        return result;
    }
}

cards::PlayResult cards::DragonSight::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_waitingForDeckChoice)
    {
        UserInputParams_ChooseDeck* responseParams = reinterpret_cast<UserInputParams_ChooseDeck*>(response.inputParams.get());
        if (!responseParams)
            return PlayResult::Default();
        if (responseParams->chosenDecks.size() != 1)
            return PlayResult::Default();

        CardType chosenDeck = responseParams->chosenDecks[0];

        std::array<bool, 2> displayedTo;
        displayedTo[actionProps.player] = true;
        displayedTo[actionProps.opponent] = false;
        core->AddCardToDisplayedCards({ core->GetState().GetDeck(chosenDeck).back().get(), displayedTo });
        int cardsRevealed = core->GetState().displayedCards.size();

        if (cardsRevealed < 4)
        {
            auto params = std::make_unique<UserInputParams_ChooseDeck>();
            params->playerIndex = actionProps.player;
            params->minDeckCount = 1;
            params->maxDeckCount = 1;
            params->allowEmptyDecks = false;

            _waitingForDeckChoice = true;

            PlayResult result;
            result.waitForInput = true;
            result.inputRequest.inputType = UserInputType::CHOOSE_DECK;
            result.inputRequest.inputPrompt = L"Select deck to reveal a card";
            result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
            return result;
        }
        else
        {
            auto params = std::make_unique<UserInputParams_WaitForConfirmation>();
            params->playerIndex = actionProps.player;

            _waitingForDeckChoice = false;
            _waitingForConfirmation = true;

            PlayResult result;
            result.waitForInput = true;
            result.inputRequest.inputType = UserInputType::WAIT_FOR_CONFIRMATION;
            result.inputRequest.inputPrompt = L"Done";
            result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
            return result;
        }
    }
    else if (_waitingForConfirmation)
    {
        _waitingForConfirmation = false;
        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}