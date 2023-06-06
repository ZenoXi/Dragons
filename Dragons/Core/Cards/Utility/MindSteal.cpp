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

    int maxCards = core->GetState().players[actionProps.opponent].hand.size();

    auto params = std::make_unique<UserInputParams_ChooseCardFromHand>();
    params->playerIndex = actionProps.player;
    params->minCardCount = 1;
    params->maxCardCount = maxCards;

    _waitingForCardChoice = true;

    PlayResult result;
    result.waitForInput = true;
    result.inputRequest.inputType = UserInputType::CHOOSE_CARD_FROM_HAND;
    result.inputRequest.inputPrompt = L"Select cards to swap";
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

        std::vector<std::unique_ptr<Card>> cardsToGive;
        for (auto& chosenCard : responseParams->chosenCards)
        {
            cardsToGive.push_back(core->RemoveCardFromHand(chosenCard, actionProps.player));
        }

        for (int i = 0; i < cardsToGive.size(); i++)
        {
            int cardIndex = core->GenerateRandomNumber(0, core->GetState().players[actionProps.opponent].hand.size());
            auto cardPtr = core->RemoveCardFromHand(cardIndex, actionProps.opponent);
            core->AddCardToHand(std::move(cardPtr), actionProps.player);
        }

        for (int i = 0; i < cardsToGive.size(); i++)
        {
            core->AddCardToHand(std::move(cardsToGive[i]), actionProps.opponent);
        }
        cardsToGive.clear();

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}