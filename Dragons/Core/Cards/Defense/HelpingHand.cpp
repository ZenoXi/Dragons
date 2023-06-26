#include "HelpingHand.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::HelpingHand::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return !core->GetState().defenseDeck.empty() && core->GetState().players[actionProps.opponent].hand.size() < GAME_HAND_SIZE;
}

cards::PlayResult cards::HelpingHand::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    Card* drawnCard = core->DrawCard(CardType::DEFENSE, actionProps.player);

    if (core->GetState().players[actionProps.player].CardsInHand(CardType::DEFENSE) == 0)
        return PlayResult::Default();

    // Request user input
    auto params = std::make_unique<UserInputParams_ChooseCardFromHand>();
    params->choosingPlayerIndex = actionProps.player;
    params->handPlayerIndex = actionProps.player;
    params->minCardCount = 1;
    params->maxCardCount = 1;
    params->allowedTypes.push_back(CardType::DEFENSE);

    _waitingToSelectCard = true;

    PlayResult result;
    result.waitForInput = true;
    result.inputRequest.inputType = UserInputType::CHOOSE_CARD_FROM_HAND;
    result.inputRequest.inputPrompt = L"Choose a defense card to give your opponent";
    result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
    return result;
}

cards::PlayResult cards::HelpingHand::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_waitingToSelectCard)
    {
        _waitingToSelectCard = false;

        UserInputParams_ChooseCardFromHand* params = reinterpret_cast<UserInputParams_ChooseCardFromHand*>(response.inputParams.get());
        if (!params)
            return PlayResult::Default();
        if (params->chosenCards.size() != 1)
            return PlayResult::Default();

        auto cardPtr = core->RemoveCardFromHand(params->chosenCards[0], actionProps.player);
        core->AddCardToHand(std::move(cardPtr), actionProps.opponent);
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}