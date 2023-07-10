#include "MagicTrick.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::MagicTrick::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::MagicTrick::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == Cheating::CARD_ID())
            _cardCheating = card;
        else if (card->GetCardId() == TheThief::CARD_ID())
            _cardTheThief = card;
    }

    if (core->GetState().players[actionProps.opponent].hand.empty())
    {
        _resumeToCardDraw = true;
        return PlayResult::Resume();
    }

    core->RevealHand(actionProps.opponent, "magic_trick");

    auto params = std::make_unique<UserInputParams_ChooseCardFromHand>();
    params->choosingPlayerIndex = actionProps.player;
    params->handPlayerIndex = actionProps.opponent;
    params->minCardCount = 1;
    params->maxCardCount = 1;

    _waitingForCardPick = true;

    PlayResult result;
    result.waitForInput = true;
    result.inputRequest.inputType = UserInputType::CHOOSE_CARD_FROM_HAND;
    result.inputRequest.inputPrompt = L"Draw a card";
    result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
    return result;
}

cards::PlayResult cards::MagicTrick::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_waitingForCardPick)
    {
        _waitingForCardPick = false;

        UserInputParams_ChooseCardFromHand* responseParams = reinterpret_cast<UserInputParams_ChooseCardFromHand*>(response.inputParams.get());
        Card* chosenCard = responseParams->chosenCards[0];

        std::vector<Card*> cardInstances;
        for (auto& card : core->GetState().players[actionProps.opponent].hand)
        {
            if (card->GetCardId() == chosenCard->GetCardId())
            {
                cardInstances.push_back(card.get());
            }
        }

        for (auto& card : cardInstances)
        {
            core->AddCardToHand(core->RemoveCardFromHand(card, actionProps.opponent), actionProps.player);
        }

        core->HideHand(actionProps.opponent, "magic_trick");

        if (cardInstances.size() == 1)
        {
            auto params = std::make_unique<UserInputParams_DrawCard>();
            params->minCardCount = 1;
            params->maxCardCount = 1;
            params->playerIndex = actionProps.player;

            _waitingForCardDraw = true;

            PlayResult result;
            result.waitForInput = true;
            result.inputRequest.inputType = UserInputType::DRAW_CARD;
            result.inputRequest.inputPrompt = L"Draw a card";
            result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
            return result;
        }

        _resumeToCleanUp = true;
        return PlayResult::Resume();
    }
    else if (_waitingForCardDraw)
    {
        _waitingForCardDraw = false;

        _resumeToCleanUp = true;
        return PlayResult::Resume();
    }
    else if (_resumeToCleanUp)
    {
        _resumeToCleanUp = false;

        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardCheating));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardTheThief));

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}