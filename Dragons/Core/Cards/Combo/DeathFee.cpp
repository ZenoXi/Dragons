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
            _cardDeathPoison = std::move(card);
        else if (card->GetCardId() == HelpingHand::CARD_ID())
            _cardHelpingHand = std::move(card);
        else if (card->GetCardId() == SummonDead::CARD_ID())
            _cardSummonDead = std::move(card);
    }
    cardsForCombo.clear();

    // Play Death Poison
    _cardDeathPoison->Play(core, actionProps, nullptr);

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

        int cardsToFullOpponentHand = GAME_HAND_SIZE - core->GetState().players[actionProps.opponent].hand.size();
        if (cardsToFullOpponentHand < 1)
        {
            _resumeToCleanUp = true;
            return PlayResult::Resume();
        }
        int defenseCardsInHand = core->GetState().players[actionProps.player].CardsInHand(CardType::DEFENSE);
        auto& graveyard = core->GetState().graveyard;
        int defenseCardsInGraveyard = std::count_if(graveyard.begin(), graveyard.end(), [](std::unique_ptr<Card>& card) { return card->GetCardType() == CardType::DEFENSE; });
        int defenseCardsAvailable = defenseCardsInHand + defenseCardsInGraveyard;

        int maxCardsToPick = cardsToFullOpponentHand;
        if (defenseCardsAvailable < maxCardsToPick)
            maxCardsToPick = defenseCardsAvailable;

        auto params = std::make_unique<UserInputParams_ChooseCardFromSet>();
        params->playerIndex = actionProps.player;
        params->minCardCount = 0;
        params->maxCardCount = maxCardsToPick;
        params->allowedSets.push_back(CardSet{ CardSets::HAND, actionProps.player });
        params->allowedSets.push_back(CardSet{ CardSets::GRAVEYARD });
        params->allowedTypes.push_back(CardType::DEFENSE);

        _waitingToSelectCards = true;

        PlayResult result;
        result.waitForInput = true;
        result.inputRequest.inputType = UserInputType::CHOOSE_CARD_FROM_SET;
        result.inputRequest.inputPrompt = L"Choose defense cards to give your opponent";
        result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
        return result;
    }
    else if (_waitingToSelectCards)
    {
        _waitingToSelectCards = false;
        UserInputParams_ChooseCardFromSet* params = reinterpret_cast<UserInputParams_ChooseCardFromSet*>(response.inputParams.get());
        if (!params)
        {
            _resumeToCleanUp = true;
            return PlayResult::Resume();
        }
        if (params->chosenCards.size() == 0)
        {
            _resumeToCleanUp = true;
            return PlayResult::Resume();
        }

        for (int i = 0; i < params->chosenCards.size(); i++)
        {
            if (params->chosenCardSet[i].set == CardSets::HAND)
            {
                auto cardPtr = core->RemoveCardFromHand(params->chosenCards[i], actionProps.player);
                core->AddCardToHand(std::move(cardPtr), actionProps.opponent);
            }
            else if (params->chosenCardSet[i].set == CardSets::GRAVEYARD)
            {
                auto cardPtr = core->RemoveCardFromGraveyard(params->chosenCards[i]);
                core->AddCardToHand(std::move(cardPtr), actionProps.opponent);
            }
        }

        DamageProperties damageProps;
        damageProps.amount = params->chosenCards.size();
        damageProps.source = actionProps.player;
        damageProps.target = actionProps.opponent;
        core->Damage(damageProps);

        _resumeToCleanUp = true;
        return PlayResult::Resume();
    }
    else if (_resumeToCleanUp)
    {
        _resumeToCleanUp = false;

        core->AddCardToGraveyard(std::move(_cardDeathPoison));
        core->AddCardToGraveyard(std::move(_cardSummonDead));
        core->AddCardToGraveyard(std::move(_cardHelpingHand));

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}