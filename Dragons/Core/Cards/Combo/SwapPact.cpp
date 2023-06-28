#include "SwapPact.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::SwapPact::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::SwapPact::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == StarCurse::CARD_ID())
            _cardStarCurse = card;
        else if (card->GetCardId() == WarBlessing::CARD_ID())
            _cardWarBlessing = card;
        else if (card->GetCardId() == MindSteal::CARD_ID())
            _cardMindSteal = card;
    }

    // Play Mind Steal
    PlayResult result = _cardMindSteal->Play(core, actionProps, nullptr);
    if (result.waitForInput)
    {
        _resumeMindSteal = true;
        return result;
    }

    _resumeToSwapChoice = true;
    return PlayResult::Resume();
}

cards::PlayResult cards::SwapPact::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
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

        _resumeToSwapChoice = true;
        return PlayResult::Resume();
    }
    else if (_resumeToSwapChoice)
    {
        _resumeToSwapChoice = false;

        auto params = std::make_unique<UserInputParams_YesOrNo>();
        params->playerIndex = actionProps.player;

        _waitingForSwapChoice = true;

        PlayResult result;
        result.waitForInput = true;
        result.inputRequest.inputType = UserInputType::CHOOSE_CARD_FROM_ACTIVE_CARDS;
        result.inputRequest.inputPrompt = L"Do you want to swap max health with your opponent?";
        result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
        return result;
    }
    else if (_waitingForSwapChoice)
    {
        _waitingForSwapChoice = false;

        UserInputParams_YesOrNo* responseParams = reinterpret_cast<UserInputParams_YesOrNo*>(response.inputParams.get());
        if (responseParams->yes)
        {
            int newPlayerMaxHealth = core->GetState().players[actionProps.opponent].maxHealth;
            int newOpponentMaxHealth = core->GetState().players[actionProps.player].maxHealth;

            core->SetMaxHealth(actionProps.player, newPlayerMaxHealth, true);
            core->SetMaxHealth(actionProps.opponent, newOpponentMaxHealth, true);
        }
    }
    else if (_resumeToStarCurse)
    {
        _resumeToStarCurse = false;

        _cardStarCurse->Play(core, actionProps, nullptr);
        _cardWarBlessing->Play(core, actionProps, nullptr);

        _resumeToCleanUp = true;
        return PlayResult::Resume();
    }
    else if (_resumeToCleanUp)
    {
        _resumeToCleanUp = false;

        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardStarCurse));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardWarBlessing));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardMindSteal));

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}