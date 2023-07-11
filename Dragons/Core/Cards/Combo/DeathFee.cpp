#include "DeathFee.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::DeathFee::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::DeathFee::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == DeathPoison::CARD_ID())
            _cardDeathPoison = card;
        else if (card->GetCardId() == HelpingHand::CARD_ID())
            _cardHelpingHand = card;
        else if (card->GetCardId() == SummonDead::CARD_ID())
            _cardSummonDead = card;
    }

    // Play Death Poison
    _cardDeathPoison->Play(core, actionProps, nullptr);
    core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardDeathPoison), actionProps.player);

    // Play Summon Dead
    if (_cardSummonDead->CanPlay(core, actionProps, nullptr))
    {
        PlayResult result = _cardSummonDead->Play(core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeSummonDead = true;
            return result;
        }
    }

    _resumeToHelpingHand = true;
    return PlayResult::Resume();
}

cards::PlayResult cards::DeathFee::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_resumeSummonDead)
    {
        _resumeSummonDead = false;
        _cardSummonDead->Resume(std::move(response), core, actionProps, nullptr);

        _resumeToHelpingHand = true;
        return PlayResult::Resume();
    }
    else if (_resumeToHelpingHand)
    {
        _resumeToHelpingHand = false;

        core->DrawCard(CardType::DEFENSE, actionProps.player, false);

        int cardsToFullOpponentHand = GAME_HAND_SIZE - core->GetState().players[actionProps.opponent].hand.size();
        if (cardsToFullOpponentHand < 1)
        {
            _resumeToCleanUp = true;
            return PlayResult::Resume();
        }
        int defenseCardsInHand = core->GetState().players[actionProps.player].CardsInHand(CardType::DEFENSE);
        auto& graveyard = core->GetState().graveyard;
        int defenseCardsInGraveyard = std::count_if(graveyard.begin(), graveyard.end(), [](const std::unique_ptr<Card>& card) { return card->GetCardType() == CardType::DEFENSE; });
        int defenseCardsAvailable = defenseCardsInHand + defenseCardsInGraveyard;

        int maxCardsToPick = cardsToFullOpponentHand;
        if (defenseCardsAvailable < maxCardsToPick)
            maxCardsToPick = defenseCardsAvailable;

        std::array<bool, 2> displayedTo;
        displayedTo[actionProps.player] = true;
        displayedTo[actionProps.opponent] = false;
        for (auto& card : core->GetState().players[actionProps.player].hand)
        {
            if (card->GetCardType() == CardType::DEFENSE)
            {
                core->AddCardToDisplayedCards({ card.get(), displayedTo });
            }
        }
        for (auto& card : core->GetState().graveyard)
        {
            if (card->GetCardType() == CardType::DEFENSE)
            {
                core->AddCardToDisplayedCards({ card.get(), displayedTo });
            }
        }

        auto params = std::make_unique<UserInputParams_ChooseCardFromDisplayedCards>();
        params->playerIndex = actionProps.player;
        params->minCardCount = 0;
        params->maxCardCount = maxCardsToPick;

        _waitingToSelectCards = true;

        PlayResult result;
        result.waitForInput = true;
        result.inputRequest.inputType = UserInputType::CHOOSE_CARD_FROM_DISPLAYED_CARDS;
        result.inputRequest.inputPrompt = L"Choose defense cards to give your opponent";
        result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
        return result;
    }
    else if (_waitingToSelectCards)
    {
        _waitingToSelectCards = false;

        UserInputParams_ChooseCardFromDisplayedCards* params = reinterpret_cast<UserInputParams_ChooseCardFromDisplayedCards*>(response.inputParams.get());

        core->ClearDisplayedCards();

        DamageProperties damageProps;
        damageProps.source = actionProps.player;
        damageProps.target = actionProps.opponent;
        damageProps.sourceCard = this;
        damageProps.amount = 1;

        for (auto chosenCard : params->chosenCards)
        {
            CardSet set = core->GetCardSet(chosenCard);
            if (set.set == CardSets::HAND)
            {
                auto cardPtr = core->RemoveCardFromHand(chosenCard, actionProps.player);
                core->AddCardToHand(std::move(cardPtr), actionProps.opponent);
            }
            else if (set.set == CardSets::GRAVEYARD)
            {
                auto cardPtr = core->RemoveCardFromGraveyard(chosenCard);
                core->AddCardToHand(std::move(cardPtr), actionProps.opponent);
            }
            core->Damage(damageProps);
        }

        _resumeToCleanUp = true;
        return PlayResult::Resume();
    }
    else if (_resumeToCleanUp)
    {
        _resumeToCleanUp = false;

        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardSummonDead));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardHelpingHand));

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}