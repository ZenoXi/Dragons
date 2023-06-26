#include "InfernoLifeline.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::InfernoLifeline::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::InfernoLifeline::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == BloodDonation::CARD_ID())
            _cardBloodDonation = card;
        else if (card->GetCardId() == DivineProtection::CARD_ID())
            _cardDivineProtection = card;
        else if (card->GetCardId() == FrostFire::CARD_ID())
            _cardFrostFire = card;
    }

    // Play Frost Fire
    PlayResult result = _cardFrostFire->Play(core, actionProps, nullptr);
    if (result.waitForInput)
    {
        _resumeFrostFire = true;
        return result;
    }

    _resumeToBloodDonation = true;
    return PlayResult::Resume();
}

cards::PlayResult cards::InfernoLifeline::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_resumeFrostFire)
    {
        _resumeFrostFire = false;

        PlayResult result = _cardFrostFire->Resume(std::move(response), core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeFrostFire = true;
            return result;
        }

        _resumeToBloodDonation = true;
        return PlayResult::Resume();
    }
    else if (_resumeToBloodDonation)
    {
        _resumeToBloodDonation = false;

        PlayResult result = _cardBloodDonation->Play(core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeBloodDonation = true;
            return result;
        }

        _resumeToDivineProtection = true;
        return PlayResult::Resume();
    }
    else if (_resumeBloodDonation)
    {
        _resumeBloodDonation = false;

        PlayResult result = _cardBloodDonation->Resume(std::move(response), core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeBloodDonation = true;
            return result;
        }

        _resumeToDivineProtection = true;
        return PlayResult::Resume();
    }
    else if (_resumeToDivineProtection)
    {
        _resumeToDivineProtection = false;

        _cardDivineProtection->Play(core, actionProps, nullptr);
        core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardDivineProtection), actionProps.player);

        _resumeToComboEffect = true;
        return PlayResult::Resume();
    }
    else if (_resumeToComboEffect)
    {
        _resumeToComboEffect = false;

        core->SetHealth(actionProps.player, 1, true);

        auto params = std::make_unique<UserInputParams_ChooseDeck>();
        params->minDeckCount = 1;
        params->maxDeckCount = 1;
        params->playerIndex = actionProps.player;
        params->allowEmptyDecks = false;

        _waitingForDeckChoice = true;

        PlayResult result;
        result.waitForInput = true;
        result.inputRequest.inputType = UserInputType::CHOOSE_DECK;
        result.inputRequest.inputPrompt = L"Select a deck to choose cards from";
        result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
        return result;
    }
    else if (_waitingForDeckChoice)
    {
        _waitingForDeckChoice = false;

        UserInputParams_ChooseDeck* responseParams = reinterpret_cast<UserInputParams_ChooseDeck*>(response.inputParams.get());
        _chosenDeck = responseParams->chosenDecks[0];

        int freeSlotsInHand = GAME_HAND_SIZE - core->GetState().players[actionProps.player].hand.size();
        if (freeSlotsInHand < 1)
        {
            _resumeToCleanUp = true;
            return PlayResult::Resume();
        }

        // Show deck
        std::array<bool, 2> displayedTo;
        displayedTo[actionProps.player] = true;
        displayedTo[actionProps.opponent] = false;
        auto& deckRef = core->GetState().GetDeck(_chosenDeck);
        for (auto& card : deckRef)
            core->AddCardToDisplayedCards({ card.get(), displayedTo });

        auto params = std::make_unique<UserInputParams_ChooseCardFromDisplayedCards>();
        params->minCardCount = 0;
        params->maxCardCount = freeSlotsInHand;
        params->playerIndex = actionProps.player;

        _waitingForCardChoice = true;

        PlayResult result;
        result.waitForInput = true;
        result.inputRequest.inputType = UserInputType::CHOOSE_CARD_FROM_DISPLAYED_CARDS;
        result.inputRequest.inputPrompt = L"Select cards to keep";
        result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
        return result;
    }
    else if (_waitingForCardChoice)
    {
        _waitingForCardChoice = false;

        UserInputParams_ChooseCardFromDisplayedCards* responseParams = reinterpret_cast<UserInputParams_ChooseCardFromDisplayedCards*>(response.inputParams.get());
        std::vector<Card*> chosenCards = responseParams->chosenCards;

        core->ClearDisplayedCards();
        // Take selected cards
        for (auto& card : chosenCards)
            core->AddCardToHand(core->RemoveCardFromDeck(card), actionProps.player);
        // Discard rest
        while (!core->GetState().GetDeck(_chosenDeck).empty())
            core->AddCardToGraveyard(core->RemoveCardFromDeck(_chosenDeck, 0));

        _resumeToCleanUp = true;
        return PlayResult::Resume();
    }
    else if (_resumeToCleanUp)
    {
        _resumeToCleanUp = false;

        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardFrostFire));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardBloodDonation));

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}