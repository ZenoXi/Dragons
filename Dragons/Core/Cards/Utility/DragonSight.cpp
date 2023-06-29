#include "DragonSight.h"

#include "../../Core.h"

cards::PlayResult cards::DragonSight::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto playPropsValue = GetPlayProperties<DragonSightPlayProperties>(playProps);

    // Create deck copies to allow easier revealed/non-revealed card checking
    _deckCopies = { {}, {}, {}, {} };
    for (auto& card : core->GetState().offenseDeck)
        _deckCopies[(int)CardType::OFFENSE].push_back(card.get());
    for (auto& card : core->GetState().defenseDeck)
        _deckCopies[(int)CardType::DEFENSE].push_back(card.get());
    for (auto& card : core->GetState().utilityDeck)
        _deckCopies[(int)CardType::UTILITY].push_back(card.get());
    for (auto& card : core->GetState().comboDeck)
        _deckCopies[(int)CardType::COMBO].push_back(card.get());

    std::array<bool, 2> displayedTo;
    displayedTo[actionProps.player] = true;
    displayedTo[actionProps.opponent] = false;
    _decksRevealed = 0;
    for (auto& deck : _deckCopies)
    {
        if (deck.empty())
            continue;

        for (int i = 0; i < playPropsValue.cardRevealMultiplier; i++)
        {
            core->AddCardToDisplayedCards({ deck.back(), displayedTo });
            deck.pop_back();
            if (deck.empty())
                break;
        }
        _decksRevealed++;
    }
    
    if (_decksRevealed < 4)
    {
        auto params = std::make_unique<UserInputParams_ChooseDeck>();
        params->playerIndex = actionProps.player;
        params->minDeckCount = 1;
        params->maxDeckCount = 1;
        params->allowEmptyDecks = false;
        for (int i = 0; i < 4; i++)
            if (!_deckCopies[i].empty())
                params->allowedDecks.push_back((CardType)i);

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
    auto playPropsValue = GetPlayProperties<DragonSightPlayProperties>(playProps);

    if (_waitingForDeckChoice)
    {
        UserInputParams_ChooseDeck* responseParams = reinterpret_cast<UserInputParams_ChooseDeck*>(response.inputParams.get());
        if (!responseParams)
            return PlayResult::Default();
        if (responseParams->chosenDecks.size() != 1)
        {
            _resumeToConfirmation = true;
            return PlayResult::Resume();
        }

        CardType chosenDeck = responseParams->chosenDecks[0];

        std::array<bool, 2> displayedTo;
        displayedTo[actionProps.player] = true;
        displayedTo[actionProps.opponent] = false;

        auto& deck = _deckCopies[(int)chosenDeck];
        for (int i = 0; i < playPropsValue.cardRevealMultiplier; i++)
        {
            core->AddCardToDisplayedCards({ deck.back(), displayedTo });
            deck.pop_back();
            if (deck.empty())
                break;
        }
        _decksRevealed++;

        if (_decksRevealed < 4)
        {
            auto params = std::make_unique<UserInputParams_ChooseDeck>();
            params->playerIndex = actionProps.player;
            params->minDeckCount = 1;
            params->maxDeckCount = 1;
            params->allowEmptyDecks = false;
            for (int i = 0; i < 4; i++)
                if (!_deckCopies[i].empty())
                    params->allowedDecks.push_back((CardType)i);

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
            _resumeToConfirmation = true;
            return PlayResult::Resume();
        }
    }
    else if (_resumeToConfirmation)
    {
        _resumeToConfirmation = false;

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
    else if (_waitingForConfirmation)
    {
        _waitingForConfirmation = false;

        core->ClearDisplayedCards();
        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}