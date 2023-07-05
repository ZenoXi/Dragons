#include "DevilsDeal.h"

#include "../../Core.h"
#include "../../GameConstants.h"

cards::PlayResult cards::DevilsDeal::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    int offenseCardsInOpponentHand = core->GetState().players[actionProps.opponent].CardsInHand(CardType::OFFENSE);
    if (offenseCardsInOpponentHand == 0)
    {
        _chosenCard = core->DrawCard(CardType::OFFENSE, actionProps.opponent, false);
        if (!_chosenCard)
            return PlayResult::Default();

        _resumeToCardPlay = true;
        return PlayResult::Resume();
    }

    // Request user input
    auto params = std::make_unique<UserInputParams_ChooseCardFromHand>();
    params->choosingPlayerIndex = actionProps.opponent;
    params->handPlayerIndex = actionProps.opponent;
    params->minCardCount = 1;
    params->maxCardCount = 1;
    params->allowedTypes.push_back(CardType::OFFENSE);

    _waitingForCardChoice = true;

    PlayResult result;
    result.waitForInput = true;
    result.inputRequest.inputType = UserInputType::CHOOSE_CARD_FROM_HAND;
    result.inputRequest.inputPrompt = L"Select offense card to play on yourself";
    result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
    return result;
}

cards::PlayResult cards::DevilsDeal::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_waitingForCardChoice)
    {
        UserInputParams_ChooseCardFromHand* responseParams = reinterpret_cast<UserInputParams_ChooseCardFromHand*>(response.inputParams.get());
        _chosenCard = responseParams->chosenCards[0];

        _resumeToCardPlay = true;
        return PlayResult::Resume();
    }
    else if (_resumeToCardPlay)
    {
        core->AddCardToInPlayCards(core->RemoveCardFromHand(_chosenCard, actionProps.opponent));
        ActionProperties customActionProps = actionProps;
        customActionProps.player = actionProps.opponent;
        customActionProps.opponent = actionProps.player;
        PlayResult result = _chosenCard->Play(core, customActionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeCardPlay = true;
            return result;
        }

        core->MoveCardAfterPlay(result, _chosenCard, customActionProps, playProps);
        return PlayResult::Default();
    }
    else if (_resumeCardPlay)
    {
        _resumeCardPlay = false;

        ActionProperties customActionProps = actionProps;
        customActionProps.player = actionProps.opponent;
        customActionProps.opponent = actionProps.player;
        PlayResult result = _chosenCard->Resume(std::move(response), core, customActionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeCardPlay = true;
            return result;
        }

        core->MoveCardAfterPlay(result, _chosenCard, customActionProps, playProps);
        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}