#include "WeaponOfChoice.h"

#include "../../Core.h"

cards::PlayResult cards::WeaponOfChoice::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    std::array<bool, 2> displayedTo;
    displayedTo[actionProps.player] = true;
    displayedTo[actionProps.opponent] = false;
    for (int i = 0; i < 3; i++)
    {
        if (!core->GetState().comboDeck.empty())
            core->AddCardToDisplayedCards({ core->GetState().comboDeck.back().get(), displayedTo });
    }

    auto params = std::make_unique<UserInputParams_ChooseCardFromDisplayedCards>();
    params->playerIndex = actionProps.player;
    params->minCardCount = 1;
    params->maxCardCount = 1;

    _waitingForCardChoice = true;

    PlayResult result;
    result.waitForInput = true;
    result.inputRequest.inputType = UserInputType::CHOOSE_CARD_FROM_DISPLAYED_CARDS;
    result.inputRequest.inputPrompt = L"Select card to keep";
    result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
    return result;
}

cards::PlayResult cards::WeaponOfChoice::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_waitingForCardChoice)
    {
        UserInputParams_ChooseCardFromDisplayedCards* responseParams = reinterpret_cast<UserInputParams_ChooseCardFromDisplayedCards*>(response.inputParams.get());
        if (!responseParams)
            return PlayResult::Default();
        if (responseParams->chosenCards.size() != 1)
            return PlayResult::Default();

        Card* chosenCard = responseParams->chosenCards[0];

        core->ClearDisplayedCards();
        auto cardPtr = core->RemoveCardFromDeck(chosenCard);
        core->AddCardToHand(std::move(cardPtr), actionProps.player);

        _waitingForCardChoice = false;
        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}