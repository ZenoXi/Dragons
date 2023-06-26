#include "LifeExchange.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::LifeExchange::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->GetState().players[actionProps.player].hand.size() < GAME_HAND_SIZE;
}

cards::PlayResult cards::LifeExchange::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (core->GetState().players[actionProps.player].hand.size() >= GAME_HAND_SIZE)
        return PlayResult::NotPlayed();

    auto params = std::make_unique<UserInputParams_DrawCard>();
    params->minCardCount = 3;
    params->maxCardCount = 3;
    params->playerIndex = actionProps.player;

    _waitingForCardDraw = true;

    PlayResult result;
    result.waitForInput = true;
    result.inputRequest.inputType = UserInputType::DRAW_CARD;
    result.inputRequest.inputPrompt = L"Draw cards";
    result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
    return result;
}

cards::PlayResult cards::LifeExchange::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_waitingForCardDraw)
    {
        _waitingForCardDraw = false;

        UserInputParams_DrawCard* responseParams = reinterpret_cast<UserInputParams_DrawCard*>(response.inputParams.get());
        if (!responseParams)
            return PlayResult::Default();

        auto params = std::make_unique<UserInputParams_ChooseCardFromHand>();
        params->choosingPlayerIndex = actionProps.player;
        params->handPlayerIndex = actionProps.player;
        params->minCardCount = 2;
        params->maxCardCount = 2;

        _waitingForCardChoice = true;

        PlayResult result;
        result.waitForInput = true;
        result.inputRequest.inputType = UserInputType::CHOOSE_CARD_FROM_HAND;
        result.inputRequest.inputPrompt = L"Choose cards to discard";
        result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
        return result;
    }
    else if (_waitingForCardChoice)
    {
        _waitingForCardChoice = false;

        UserInputParams_ChooseCardFromHand* responseParams = reinterpret_cast<UserInputParams_ChooseCardFromHand*>(response.inputParams.get());
        if (!responseParams)
            return PlayResult::Default();
        if (responseParams->chosenCards.empty())
            return PlayResult::Default();

        for (auto& chosenCard : responseParams->chosenCards)
        {
            auto cardPtr = core->RemoveCardFromHand(chosenCard, actionProps.player);
            core->AddCardToGraveyard(std::move(cardPtr));
        }

        return PlayResult::Default();
    }

    return PlayResult::NotPlayed();
}