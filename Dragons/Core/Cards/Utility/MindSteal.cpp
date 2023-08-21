#include "MindSteal.h"

#include "../../Core.h"

bool cards::MindSteal::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return !core->GetState().players[actionProps.opponent].hand.empty();
}

cards::PlayResult cards::MindSteal::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (core->GetState().players[actionProps.opponent].hand.empty())
        return PlayResult::Default();

    int maxCards = std::min(core->GetState().players[actionProps.player].hand.size(), core->GetState().players[actionProps.opponent].hand.size());

    auto params = std::make_unique<UserInputParams_ChooseCardFromHand>();
    params->choosingPlayerIndex = actionProps.player;
    params->handPlayerIndex = actionProps.player;
    params->minCardCount = 1;
    params->maxCardCount = maxCards;

    _waitingForCardChoice = true;

    PlayResult result;
    result.waitForInput = true;
    result.inputRequest.inputType = UserInputType::CHOOSE_CARD_FROM_HAND;
    result.inputRequest.inputPrompt = L"Select cards to give";
    result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
    return result;
}

cards::PlayResult cards::MindSteal::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_waitingForCardChoice)
    {
        _waitingForCardChoice = false;

        UserInputParams_ChooseCardFromHand* responseParams = reinterpret_cast<UserInputParams_ChooseCardFromHand*>(response.inputParams.get());
        if (!responseParams)
            return PlayResult::Default();
        if (responseParams->chosenCards.empty())
            return PlayResult::Default();
        if (responseParams->chosenCards.size() > core->GetState().players[actionProps.opponent].hand.size())
            return PlayResult::Default();

        _chosenCardsToGive = responseParams->chosenCards;

        auto params = std::make_unique<UserInputParams_ChooseCardFromHand>();
        params->choosingPlayerIndex = actionProps.player;
        params->handPlayerIndex = actionProps.opponent;
        params->minCardCount = _chosenCardsToGive.size();
        params->maxCardCount = _chosenCardsToGive.size();

        _waitingForOpponentCardChoice = true;

        PlayResult result;
        result.waitForInput = true;
        result.inputRequest.inputType = UserInputType::CHOOSE_CARD_FROM_HAND;
        result.inputRequest.inputPrompt = L"Select cards to take";
        result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
        return result;
    }
    else if (_waitingForOpponentCardChoice)
    {
        _waitingForOpponentCardChoice = false;

        UserInputParams_ChooseCardFromHand* responseParams = reinterpret_cast<UserInputParams_ChooseCardFromHand*>(response.inputParams.get());
        if (!responseParams)
            return PlayResult::Default();
        if (responseParams->chosenCards.empty())
            return PlayResult::Default();
        if (responseParams->chosenCards.size() != _chosenCardsToGive.size())
            return PlayResult::Default();

        for (auto& chosenCard : _chosenCardsToGive)
            core->AddCardToHand(core->RemoveCardFromHand(chosenCard, actionProps.player), actionProps.opponent);

        for (auto& chosenCard : responseParams->chosenCards)
            core->AddCardToHand(core->RemoveCardFromHand(chosenCard, actionProps.opponent), actionProps.player);

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}