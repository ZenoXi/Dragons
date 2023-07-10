#include "UltimateExchange.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::UltimateExchange::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::UltimateExchange::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == MindSteal::CARD_ID())
            _cardMindSteal = card;
        else if (card->GetCardId() == DragonEquation::CARD_ID())
            _cardDragonEquation = card;
    }

    // Play Mind Steal
    PlayResult result = _cardMindSteal->Play(core, actionProps, nullptr);
    if (result.waitForInput)
    {
        _resumeMindSteal = true;
        return result;
    }

    _resumeToDragonEquation = true;
    return PlayResult::Resume();
}

cards::PlayResult cards::UltimateExchange::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_resumeMindSteal)
    {
        _resumeMindSteal = false;

        PlayResult result = _cardMindSteal->Resume(std::move(response), core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeMindSteal = true;
            return result;
        }

        _resumeToDragonEquation = true;
        return PlayResult::Resume();
    }
    else if (_resumeToDragonEquation)
    {
        _resumeToDragonEquation = false;

        _cardDragonEquation->Play(core, actionProps, nullptr);

        auto params = std::make_unique<UserInputParams_YesOrNo>();
        params->playerIndex = actionProps.player;

        _waitingForHealthSwapChoice = true;

        PlayResult result;
        result.waitForInput = true;
        result.inputRequest.inputType = UserInputType::YES_OR_NO;
        result.inputRequest.inputPrompt = L"Do you want to swap health with your opponent?";
        result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
        return result;
    }
    else if (_waitingForHealthSwapChoice)
    {
        _waitingForHealthSwapChoice = false;

        UserInputParams_YesOrNo* responseParams = reinterpret_cast<UserInputParams_YesOrNo*>(response.inputParams.get());
        if (responseParams->yes)
        {
            int newPlayerHealth = core->GetState().players[actionProps.opponent].health;
            int newOpponentHealth = core->GetState().players[actionProps.player].health;

            if (newPlayerHealth > core->GetState().players[actionProps.player].maxHealth)
                newPlayerHealth = core->GetState().players[actionProps.player].maxHealth;
            if (newOpponentHealth > core->GetState().players[actionProps.opponent].maxHealth)
                newOpponentHealth = core->GetState().players[actionProps.opponent].maxHealth;

            core->SetHealth(actionProps.player, newPlayerHealth, true);
            core->SetHealth(actionProps.opponent, newOpponentHealth, true);
        }

        _resumeToCleanUp = true;
        return PlayResult::Resume();
    }
    else if (_resumeToCleanUp)
    {
        _resumeToCleanUp = false;

        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardMindSteal));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardDragonEquation));

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}