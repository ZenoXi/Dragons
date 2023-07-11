#include "FrostFire.h"

#include "../../Core.h"

cards::PlayResult cards::FrostFire::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    // Request user input
    auto params = std::make_unique<UserInputParams_ChooseDeck>();
    params->playerIndex = actionProps.player;
    params->minDeckCount = 1;
    params->maxDeckCount = 1;
    params->allowEmptyDecks = false;

    _waitingForDeckChoice = true;

    PlayResult result;
    result.waitForInput = true;
    result.inputRequest.inputType = UserInputType::CHOOSE_DECK;
    result.inputRequest.inputPrompt = L"Select deck to destroy cards from";
    result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
    return result;
}

cards::PlayResult cards::FrostFire::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_waitingForDeckChoice)
    {
        UserInputParams_ChooseDeck* params = reinterpret_cast<UserInputParams_ChooseDeck*>(response.inputParams.get());
        if (!params)
            return PlayResult::Default();
        if (params->chosenDecks.size() != 1)
            return PlayResult::Default();

        CardType chosenDeck = params->chosenDecks[0];

        int cardsToDestroy = std::min(5, (int)core->GetState().GetDeck(chosenDeck).size());
        for (int i = 0; i < cardsToDestroy; i++)
        {
            Card* topCard = core->GetState().GetDeck(chosenDeck).back().get();
            auto cardPtr = core->RemoveCardFromDeck(topCard);
            core->AddCardToDestroyedCards(std::move(cardPtr));
        }

        _waitingForDeckChoice = false;
        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}