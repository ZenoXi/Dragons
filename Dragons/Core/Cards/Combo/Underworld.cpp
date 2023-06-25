#include "Underworld.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::Underworld::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::Underworld::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == SummonDead::CARD_ID())
            _cardSummonDead = card;
        else if (card->GetCardId() == DragonFlame::CARD_ID())
            _cardDragonFlame = card;
    }

    // Play Summon Dead
    PlayResult result = _cardSummonDead->Play(core, actionProps, nullptr);
    if (result.waitForInput)
    {
        _resumeSummonDead = true;
        return result;
    }

    _resumeToDragonFlame = true;
    return PlayResult::Resume();
}

cards::PlayResult cards::Underworld::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_resumeSummonDead)
    {
        _resumeSummonDead = false;

        PlayResult result = _cardSummonDead->Resume(std::move(response), core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeSummonDead = true;
            return result;
        }

        _resumeToDragonFlame = true;
        return PlayResult::Resume();
    }
    else if (_resumeToDragonFlame)
    {
        _resumeToDragonFlame = false;

        auto params = std::make_unique<UserInputParams_ChooseCardFromActiveCards>();
        params->choosingPlayerIndex = actionProps.player;
        params->targetActivesPlayerIndex = actionProps.opponent;
        params->minCardCount = 0;
        int freeSlotsInHand = GAME_HAND_SIZE - core->GetState().players[actionProps.player].hand.size();
        if (freeSlotsInHand < 0)
            freeSlotsInHand = 0;
        params->maxCardCount = freeSlotsInHand;

        _waitingForCardChoice = true;

        PlayResult result;
        result.waitForInput = true;
        result.inputRequest.inputType = UserInputType::CHOOSE_CARD_FROM_ACTIVE_CARDS;
        result.inputRequest.inputPrompt = L"Select cards to keep";
        result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
        return result;
    }
    else if (_waitingForCardChoice)
    {
        _waitingForCardChoice = false;

        UserInputParams_ChooseCardFromActiveCards* responseParams = reinterpret_cast<UserInputParams_ChooseCardFromActiveCards*>(response.inputParams.get());
        std::vector<Card*> chosenCards = responseParams->chosenCards;

        // Take selected cards
        for (auto& card : chosenCards)
            core->AddCardToHand(core->RemoveCardFromActiveCards(card, actionProps.opponent), actionProps.player);

        // Destroy the rest
        while (!core->GetState().players[actionProps.opponent].activeCards.empty())
            core->AddCardToDestroyedCards(core->RemoveCardFromActiveCards(0, actionProps.opponent));

        _resumeToCleanUp = true;
        return PlayResult::Resume();
    }
    else if (_resumeToCleanUp)
    {
        _resumeToCleanUp = false;

        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardSummonDead));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardDragonFlame));

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}