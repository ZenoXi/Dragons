#include "LastBreath.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::LastBreath::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->GetState().players[actionProps.player].hand.size() < core->GetState().players[actionProps.opponent].hand.size();
}

cards::PlayResult cards::LastBreath::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    // Request user input
    auto params = std::make_unique<UserInputParams_PlayCard>();
    params->minCardCount = 99;
    params->maxCardCount = 99;

    _waitingToPlayCards = true;

    PlayResult result;
    result.waitForInput = true;
    result.inputRequest.inputType = UserInputType::PLAY_CARD;
    result.inputRequest.inputPrompt = L"Play all offensive cards";
    result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
    return result;
}

cards::PlayResult cards::LastBreath::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_waitingToPlayCards)
    {
        _waitingToPlayCards = false;

        // Discard remaining cards
        auto& handRef = core->GetState().players[actionProps.player].hand;
        while (!handRef.empty())
            core->DiscardCard(handRef.front().get(), actionProps.player);

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}