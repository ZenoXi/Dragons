#include "DevilsDeal.h"

#include "../../Core.h"
#include "../../GameConstants.h"

cards::PlayResult cards::DevilsDeal::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    int offenseCardsInHand = core->GetState().players[actionProps.player].CardsInHand(CardType::OFFENSE);
    if (offenseCardsInHand == 0)
        return PlayResult::Default();

    // Request user input
    auto params = std::make_unique<UserInputParams_ChooseCardFromHand>();
    params->choosingPlayerIndex = actionProps.player;
    params->handPlayerIndex = actionProps.player;
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
        Card* chosenCard = responseParams->chosenCards[0];

        core->AddCardToInPlayCards(core->RemoveCardFromHand(chosenCard, actionProps.opponent));
        ActionProperties customActionProps = actionProps;
        customActionProps.player = actionProps.opponent;
        customActionProps.opponent = actionProps.player;
        PlayResult result = chosenCard->Play(core, customActionProps, nullptr);

        // Discard cards
        int discardedCount = 0;
        for (auto& card : params->chosenCards)
        {
            if (core->DiscardCard(card, actionProps.player))
                discardedCount++;
        }

        // Draw cards
        for (int i = 0; i < discardedCount; i++)
        {
            core->DrawCard(CardType::OFFENSE, actionProps.player);
        }

        _waitingForCardChoice = false;
        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}