#include "Stab.h"

#include "../../Core.h"

cards::PlayResult cards::Stab::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto params = std::make_unique<UserInputParams_YesOrNo>();
    params->playerIndex = actionProps.player;
    params->yesText = L"Yay";
    params->noText = L"Ney";

    _waitingForChoice = true;

    PlayResult result;
    result.waitForInput = true;
    result.inputRequest.inputType = UserInputType::YES_OR_NO;
    result.inputRequest.inputPrompt = L"Would you like to (and yes, this is your choice) fucking obliterate your opponent?";
    result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
    return result;
}

cards::PlayResult cards::Stab::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_waitingForChoice)
    {
        _waitingForChoice = false;
        UserInputParams_YesOrNo* params = reinterpret_cast<UserInputParams_YesOrNo*>(response.inputParams.get());

        if (params->yes)
        {
            DamageProperties damageProps;
            damageProps.source = actionProps.player;
            damageProps.target = actionProps.opponent;
            damageProps.amount = 2;
            damageProps.sourceCard = this;
            core->Damage(damageProps);
        }

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}