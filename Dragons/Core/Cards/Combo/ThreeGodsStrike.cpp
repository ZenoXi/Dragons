#include "ThreeGodsStrike.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::ThreeGodsStrike::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::ThreeGodsStrike::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == SacrificialAttack::CARD_ID())
            _cardSacrificialAttack = card;
        else if (card->GetCardId() == Barrier::CARD_ID())
            _cardBarrier = card;
        else if (card->GetCardId() == DragonSight::CARD_ID())
            _cardDragonSight = card;
    }

    auto params = std::make_unique<UserInputParams_ChooseOption>();
    params->playerIndex = actionProps.player;
    params->options.push_back(L"Dragon Sight draws 4 top cards instead");
    params->options.push_back(L"Sacrificial Attack steals health instead");
    params->options.push_back(L"Barrier blocks twice and blocks Dragon Flame");

    _waitingForEffectChoice = true;

    PlayResult result;
    result.waitForInput = true;
    result.inputRequest.inputType = UserInputType::CHOOSE_OPTION;
    result.inputRequest.inputPrompt = L"Choose combo effect";
    result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
    return result;
}

cards::PlayResult cards::ThreeGodsStrike::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_waitingForEffectChoice)
    {
        _waitingForEffectChoice = false;

        _buffSacrificialAttack = false;
        _buffBarrier = false;
        _buffDragonSight = false;
        UserInputParams_ChooseOption* responseParams = reinterpret_cast<UserInputParams_ChooseOption*>(response.inputParams.get());
        if (responseParams->chosenOptionIndex == 0)
            _buffDragonSight = true;
        else if (responseParams->chosenOptionIndex == 1)
            _buffSacrificialAttack = true;
        else if (responseParams->chosenOptionIndex == 2)
            _buffBarrier = true;

        _resumeToDragonSight = true;
        return PlayResult::Resume();
    }
    else if (_resumeToDragonSight)
    {
        _resumeToDragonSight = false;

        DragonSightPlayProperties dragonSightPlayProperties;
        dragonSightPlayProperties.cardRevealMultiplier = 4;
        DragonSightPlayProperties* dragonSightPlayPropertiesPtr = nullptr;
        if (_buffDragonSight)
            dragonSightPlayPropertiesPtr = &dragonSightPlayProperties;
        PlayResult result = _cardDragonSight->Play(core, actionProps, dragonSightPlayPropertiesPtr);
        if (result.waitForInput)
        {
            _resumeDragonSight = true;
            return result;
        }

        _resumeToSacrificialAttack = true;
        return PlayResult::Resume();
    }
    else if (_resumeDragonSight)
    {
        _resumeDragonSight = false;

        DragonSightPlayProperties dragonSightPlayProperties;
        dragonSightPlayProperties.cardRevealMultiplier = 4;
        DragonSightPlayProperties* dragonSightPlayPropertiesPtr = nullptr;
        if (_buffDragonSight)
            dragonSightPlayPropertiesPtr = &dragonSightPlayProperties;
        PlayResult result = _cardDragonSight->Resume(std::move(response), core, actionProps, dragonSightPlayPropertiesPtr);
        if (result.waitForInput)
        {
            _resumeDragonSight = true;
            return result;
        }

        _resumeToSacrificialAttack = true;
        return PlayResult::Resume();
    }
    else if (_resumeToSacrificialAttack)
    {
        _resumeToSacrificialAttack = false;

        SacrificialAttackPlayProperties sacrificialAttackPlayProperties;
        sacrificialAttackPlayProperties.damageSelf = false;
        sacrificialAttackPlayProperties.stealHealth = true;
        SacrificialAttackPlayProperties* sacrificialAttackPlayPropertiesPtr = nullptr;
        if (_buffSacrificialAttack)
            sacrificialAttackPlayPropertiesPtr = &sacrificialAttackPlayProperties;
        _cardSacrificialAttack->Play(core, actionProps, sacrificialAttackPlayPropertiesPtr);

        _resumeToBarrier = true;
        return PlayResult::Resume();
    }
    else if (_resumeToBarrier)
    {
        _resumeToBarrier = false;

        BarrierPlayProperties barrierPlayProperties;
        barrierPlayProperties.blockedAttacks = 2;
        barrierPlayProperties.blockDragonFlame = true;
        BarrierPlayProperties* barrierPlayPropertiesPtr = nullptr;
        if (_buffBarrier)
            barrierPlayPropertiesPtr = &barrierPlayProperties;
        _cardBarrier->Play(core, actionProps, barrierPlayPropertiesPtr);
        core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardBarrier), actionProps.player);

        _resumeToCleanUp = true;
        return PlayResult::Resume();
    }
    else if (_resumeToCleanUp)
    {
        _resumeToCleanUp = false;

        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardDragonSight));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardSacrificialAttack));

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}