#include "WeaponScroll.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::WeaponScroll::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->GetState().players[actionProps.player].CardsInHand(CardType::OFFENSE) > 0;
}

cards::PlayResult cards::WeaponScroll::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    int offenseCardsInHand = core->GetState().players[actionProps.player].CardsInHand(CardType::OFFENSE);
    if (offenseCardsInHand == 0)
        return PlayResult::Default();

    // Request user input
    auto params = std::make_unique<UserInputParams_ChooseCardFromHand>();
    params->choosingPlayerIndex = actionProps.player;
    params->handPlayerIndex = actionProps.player;
    params->minCardCount = 0;
    params->maxCardCount = 10;
    params->allowedTypes.push_back(CardType::OFFENSE);

    _waitingForCardChoice = true;

    PlayResult result;
    result.waitForInput = true;
    result.inputRequest.inputType = UserInputType::CHOOSE_CARD_FROM_HAND;
    result.inputRequest.inputPrompt = L"Select offense cards to discard";
    result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
    return result;
}

cards::PlayResult cards::WeaponScroll::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_waitingForCardChoice)
    {
        UserInputParams_ChooseCardFromHand* params = reinterpret_cast<UserInputParams_ChooseCardFromHand*>(response.inputParams.get());
        if (!params)
            return PlayResult::Default();

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