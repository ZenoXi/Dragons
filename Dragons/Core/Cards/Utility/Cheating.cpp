#include "Cheating.h"

#include "../../Core.h"

cards::PlayResult cards::Cheating::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    core->RevealHand(actionProps.opponent);

    auto params = std::make_unique<UserInputParams_WaitForConfirmation>();
    params->playerIndex = actionProps.player;

    _waitingForConfirmation = true;

    PlayResult result;
    result.waitForInput = true;
    result.inputRequest.inputType = UserInputType::WAIT_FOR_CONFIRMATION;
    result.inputRequest.inputPrompt = L"Done";
    result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
    return result;
}

cards::PlayResult cards::Cheating::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_waitingForConfirmation)
    {
        core->HideHand(actionProps.opponent);

        _waitingForConfirmation = false;
        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}