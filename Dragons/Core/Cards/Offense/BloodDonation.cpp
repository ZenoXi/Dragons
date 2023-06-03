#include "BloodDonation.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::BloodDonation::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->GetState().players[actionProps.player].hand.size() < GAME_HAND_SIZE;
}

cards::PlayResult cards::BloodDonation::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    // Request user input
    auto params = std::make_unique<UserInputParams_ChooseNumber>();
    params->lowerBound = 0;
    params->upperBound = 99;

    _waitingForDamageInput = true;

    PlayResult result;
    result.waitForInput = true;
    result.inputRequest.inputType = UserInputType::CHOOSE_NUMBER;
    result.inputRequest.inputPrompt = L"Select damage amount";
    result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
    return result;
}

cards::PlayResult cards::BloodDonation::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_waitingForDamageInput)
    {
        UserInputParams_ChooseNumber* params = reinterpret_cast<UserInputParams_ChooseNumber*>(response.inputParams.get());
        if (!params)
            return PlayResult::Default();

        DamageProperties damageProps;
        damageProps.amount = params->chosenNumber;
        damageProps.source = -1;
        damageProps.target = actionProps.player;
        damageProps.trueDamage = true;

        DamageResult damageResult = core->Damage(damageProps);
        int cardsToDraw = damageResult.removedHealthAmount / 2;
        if (cardsToDraw == 0)
            return PlayResult::Default();

        // Request user input
        auto drawCardParams = std::make_unique<UserInputParams_DrawCard>();
        drawCardParams->playerIndex = actionProps.player;
        drawCardParams->minCardCount = cardsToDraw;
        drawCardParams->maxCardCount = cardsToDraw;

        _waitingForDamageInput = false;
        _waitingToDrawCards = true;

        PlayResult result;
        result.waitForInput = true;
        result.inputRequest.inputType = UserInputType::DRAW_CARD;
        result.inputRequest.inputPrompt = L"Draw cards";
        result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(drawCardParams.release());
        return result;
    }
    else if (_waitingToDrawCards)
    {
        _waitingToDrawCards = false;
        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}