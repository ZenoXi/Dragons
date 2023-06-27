#include "Apocalypse.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::Apocalypse::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::Apocalypse::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == LifeExchange::CARD_ID())
            _cardLifeExchange = card;
        else if (card->GetCardId() == FrostFire::CARD_ID())
            _cardFrostFire = card;
    }

    // Play Life Exchange
    PlayResult result = _cardLifeExchange->Play(core, actionProps, nullptr);
    if (result.waitForInput)
    {
        _resumeLifeExchange = true;
        return result;
    }

    _resumeToFrostFire = true;
    return PlayResult::Resume();
}

cards::PlayResult cards::Apocalypse::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_resumeLifeExchange)
    {
        _resumeLifeExchange = false;

        PlayResult result = _cardLifeExchange->Resume(std::move(response), core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeLifeExchange = true;
            return result;
        }

        _resumeToFrostFire = true;
        return PlayResult::Resume();
    }
    else if (_resumeToFrostFire)
    {
        _resumeToFrostFire = false;

        PlayResult result = _cardFrostFire->Play(core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeFrostFire = true;
            return result;
        }

        _resumeToCardDraw = true;
        return PlayResult::Resume();
    }
    else if (_resumeFrostFire)
    {
        _resumeFrostFire = false;

        PlayResult result = _cardFrostFire->Resume(std::move(response), core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeFrostFire = true;
            return result;
        }

        _resumeToCardDraw = true;
        return PlayResult::Resume();
    }
    else if (_resumeToCardDraw)
    {
        _resumeToCardDraw = false;

        auto params = std::make_unique<UserInputParams_DrawCard>();
        params->playerIndex = actionProps.player;
        params->minCardCount = 10;
        params->maxCardCount = 10;
        params->ignoreHandSize = true;

        _waitingForCardDraw = true;

        PlayResult result;
        result.waitForInput = true;
        result.inputRequest.inputType = UserInputType::DRAW_CARD;
        result.inputRequest.inputPrompt = L"Draw 10 cards";
        result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
        return result;
    }
    else if (_waitingForCardDraw)
    {
        _waitingForCardDraw = false;

        int requiredCardDiscardAmount = core->GetState().players[actionProps.player].hand.size() - GAME_HAND_SIZE;
        if (requiredCardDiscardAmount < 0)
            requiredCardDiscardAmount = 0;

        auto params = std::make_unique<UserInputParams_DiscardCard>();
        params->playerIndex = actionProps.player;
        params->minCardCount = requiredCardDiscardAmount;
        params->maxCardCount = 99;

        _waitingForCardDraw = true;

        PlayResult result;
        result.waitForInput = true;
        result.inputRequest.inputType = UserInputType::DISCARD_CARD;
        result.inputRequest.inputPrompt = L"Discard cards until you have 10 or less cards left";
        result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
        return result;
    }
    else if (_waitingForCardDiscard)
    {
        _waitingForCardDiscard = false;

        _resumeToCleanUp = true;
        return PlayResult::Resume();
    }
    else if (_resumeToCleanUp)
    {
        _resumeToCleanUp = false;

        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardLifeExchange));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardFrostFire));

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}