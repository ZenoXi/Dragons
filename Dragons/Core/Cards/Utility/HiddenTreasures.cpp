#include "HiddenTreasures.h"

#include "../../Core.h"

cards::PlayResult cards::HiddenTreasures::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
        auto params = std::make_unique<UserInputParams_DrawCard>();
        params->minCardCount = 2;
        params->maxCardCount = 2;
        params->playerIndex = actionProps.player;

        _waitingForCardDraw = true;

        PlayResult result;
        result.waitForInput = true;
        result.inputRequest.inputType = UserInputType::DRAW_CARD;
        result.inputRequest.inputPrompt = L"Draw 2 cards";
        result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
        return result;
}

cards::PlayResult cards::HiddenTreasures::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_waitingForCardDraw)
    {
        _waitingForCardDraw = false;

        UserInputParams_DrawCard* responseParams = reinterpret_cast<UserInputParams_DrawCard*>(response.inputParams.get());
        if (!responseParams)
            return PlayResult::Default();

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}