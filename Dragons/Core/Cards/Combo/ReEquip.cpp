#include "ReEquip.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::ReEquip::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::ReEquip::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == WeaponScroll::CARD_ID())
            _cardWeaponScroll = card;
        else if (card->GetCardId() == PeacePact::CARD_ID())
            _cardPeacePact = card;
        else if (card->GetCardId() == WeaponOfChoice::CARD_ID())
            _cardWeaponOfChoice = card;
    }

    // Play Weapon of Choice
    PlayResult result = _cardWeaponOfChoice->Play(core, actionProps, nullptr);
    if (result.waitForInput)
    {
        _resumeWeaponOfChoice = true;
        return result;
    }

    _resumeToWeaponScroll = true;
    return PlayResult::Resume();
}

cards::PlayResult cards::ReEquip::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_resumeWeaponOfChoice)
    {
        _resumeWeaponOfChoice = false;

        PlayResult result = _cardWeaponOfChoice->Resume(std::move(response), core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeWeaponOfChoice = true;
            return result;
        }

        _resumeToWeaponScroll = true;
        return PlayResult::Resume();
    }
    else if (_resumeToWeaponScroll)
    {
        _resumeToWeaponScroll = false;

        _ignoreCard = false;
        _discardedOffenseCards.clear();
        _cardLeaveHandHandler = std::make_unique<EventHandler<CardLeaveHandEvent>>(&core->Events(), [=](CardLeaveHandEvent event)
        {
            if (event.playerIndex == actionProps.opponent)
                return;
            if (event.card->GetCardType() != CardType::OFFENSE)
                return;

            _discardedOffenseCards.push_back(event.card);
        });

        PlayResult result = _cardWeaponScroll->Play(core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeWeaponScroll = true;
            return result;
        }

        _resumeToPeacePact = true;
        return PlayResult::Resume();
    }
    else if (_resumeWeaponScroll)
    {
        _resumeWeaponScroll = false;

        PlayResult result = _cardWeaponScroll->Resume(std::move(response), core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeWeaponScroll = true;
            return result;
        }

        _resumeToPeacePact = true;
        return PlayResult::Resume();
    }
    else if (_resumeToPeacePact)
    {
        _resumeToPeacePact = false;

        _cardPeacePact->Play(core, actionProps, nullptr);
        _cardLeaveHandHandler.reset();

        _resumeToCardChoice = true;
        return PlayResult::Resume();
    }
    else if (_resumeToCardChoice)
    {
        _resumeToCardChoice = false;

        std::array<bool, 2> displayedTo;
        displayedTo[actionProps.player] = true;
        displayedTo[actionProps.opponent] = false;
        for (auto& card : _discardedOffenseCards)
            core->AddCardToDisplayedCards({ card, displayedTo });

        int freeSlotsInHand = GAME_HAND_SIZE - core->GetState().players[actionProps.player].hand.size();
        if (freeSlotsInHand < 1)
        {
            _resumeToCleanUp = true;
            return PlayResult::Resume();
        }

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
        {
            CardSet set = core->GetCardSet(card);
            if (set.set == CardSets::GRAVEYARD)
                core->AddCardToHand(core->RemoveCardFromGraveyard(card), actionProps.player);
            else if (set.set == CardSets::DECK)
                core->AddCardToHand(core->RemoveCardFromDeck(card), actionProps.player);
        }

        _resumeToCleanUp = true;
        return PlayResult::Resume();
    }
    else if (_resumeToCleanUp)
    {
        _resumeToCleanUp = false;

        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardWeaponOfChoice));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardWeaponScroll));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardPeacePact));

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}