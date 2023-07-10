#include "Vampirism.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::Vampirism::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::Vampirism::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == DevilsDeal::CARD_ID())
            _cardDevilsDeal = card;
        else if (card->GetCardId() == BloodDonation::CARD_ID())
            _cardBloodDonation = card;
    }

    _damageToSelf = 0;
    _postDamageHandler = std::make_unique<EventHandler<PostDamageEvent>>(&core->Events(), [=](PostDamageEvent event)
    {
        _damageToSelf = event.result.removedHealthAmount;
    });

    // Play Blood Donation
    PlayResult result = _cardBloodDonation->Play(core, actionProps, &_bloodDonationPlayProperties);
    if (result.waitForInput)
    {
        _resumeBloodDonation = true;
        return result;
    }

    _resumeToDevilsDeal = true;
    return PlayResult::Resume();
}

cards::PlayResult cards::Vampirism::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_resumeBloodDonation)
    {
        _resumeBloodDonation = false;

        PlayResult result = _cardBloodDonation->Resume(std::move(response), core, actionProps, &_bloodDonationPlayProperties);
        if (result.waitForInput)
        {
            _resumeBloodDonation = true;
            return result;
        }

        _resumeToDevilsDeal = true;
        return PlayResult::Resume();
    }
    else if (_resumeToDevilsDeal)
    {
        _resumeToDevilsDeal = false;

        _opponentDamageToSelf = 0;
        _postDamageHandler = std::make_unique<EventHandler<PostDamageEvent>>(&core->Events(), [=](PostDamageEvent event)
        {
            if (event.props.source != actionProps.opponent || event.props.target != actionProps.opponent)
                return;

            _opponentDamageToSelf += event.result.removedHealthAmount;
        });

        PlayResult result = _cardDevilsDeal->Play(core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeDevilsDeal = true;
            return result;
        }

        _resumeToCardDraw = true;
        return PlayResult::Resume();
    }
    else if (_resumeDevilsDeal)
    {
        _resumeDevilsDeal = false;

        PlayResult result = _cardDevilsDeal->Resume(std::move(response), core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeDevilsDeal = true;
            return result;
        }

        _resumeToCardDraw = true;
        return PlayResult::Resume();
    }
    else if (_resumeToCardDraw)
    {
        _resumeToCardDraw = false;

        int cardsToDraw = _damageToSelf + _opponentDamageToSelf;
        int freeSlotsInHand = GAME_HAND_SIZE - core->GetState().players[actionProps.player].hand.size();
        if (freeSlotsInHand < 0)
            freeSlotsInHand = 0;
        if (cardsToDraw > freeSlotsInHand)
            cardsToDraw = freeSlotsInHand;

        if (cardsToDraw <= 0)
        {
            _resumeToCleanUp = true;
            return PlayResult::Resume();
        }

        // Request user input
        auto drawCardParams = std::make_unique<UserInputParams_DrawCard>();
        drawCardParams->playerIndex = actionProps.player;
        drawCardParams->minCardCount = cardsToDraw;
        drawCardParams->maxCardCount = cardsToDraw;

        _waitingForCardDraw = true;

        PlayResult result;
        result.waitForInput = true;
        result.inputRequest.inputType = UserInputType::DRAW_CARD;
        result.inputRequest.inputPrompt = L"Draw cards";
        result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(drawCardParams.release());
        return result;
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

        _postDamageHandler.reset();
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardDevilsDeal));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardBloodDonation));

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}