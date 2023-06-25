#include "CounterScale.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::CounterScale::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::CounterScale::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == RapidAttack::CARD_ID())
            _cardRapidAttack = card;
        else if (card->GetCardId() == Preparations::CARD_ID())
            _cardPreparations = card;
        else if (card->GetCardId() == DragonPower::CARD_ID())
            _cardDragonPower = card;
    }

    // Play dragon power
    if (_cardDragonPower->CanPlay(core, actionProps, nullptr))
    {
        PlayResult result = _cardDragonPower->Play(core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeDragonPower = true;
            return result;
        }
    }

    _resumeToRapidAttack = true;
    return PlayResult::Resume();
}

cards::PlayResult cards::CounterScale::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_resumeDragonPower)
    {
        _resumeDragonPower = false;
        _cardDragonPower->Resume(std::move(response), core, actionProps, nullptr);

        _resumeToRapidAttack = true;
        return PlayResult::Resume();
    }
    else if (_resumeToRapidAttack)
    {
        _resumeToRapidAttack = false;

        // Play Rapid Attack
        RapidAttackPlayProperties rapidAttackPlayProps;
        rapidAttackPlayProps.ignoreRepeatPlayRestriction = true;
        _cardRapidAttack->Play(core, actionProps, &rapidAttackPlayProps);

        // Play Preparations
        _cardPreparations->Play(core, actionProps, nullptr);

        // Apply combo effect
        int comboEffectApplyCount = 0;
        if (core->GetState().offenseDeck.empty())
            comboEffectApplyCount++;
        if (core->GetState().defenseDeck.empty())
            comboEffectApplyCount++;
        if (core->GetState().utilityDeck.empty())
            comboEffectApplyCount++;
        if (core->GetState().comboDeck.empty())
            comboEffectApplyCount++;
        if (comboEffectApplyCount == 0)
        {
            _resumeToCleanUp = true;
            return PlayResult::Resume();
        }

        int armorAmount = core->GetState().players[actionProps.opponent].CardsInHand(CardType::OFFENSE);
        int damageAmount = core->GetState().players[actionProps.opponent].CardsInHand(CardType::DEFENSE);
        DamageProperties damageProps;
        damageProps.amount = damageAmount;
        damageProps.source = actionProps.player;
        damageProps.target = actionProps.opponent;
        AddArmorProperties addArmorProps;
        addArmorProps.target = actionProps.player;
        addArmorProps.amount = armorAmount;
        addArmorProps.sourceCard = this;
        for (int i = 0; i < comboEffectApplyCount; i++)
        {
            if (armorAmount > 0)
            {
                core->AddArmor(addArmorProps);
            }
            if (damageAmount > 0)
            {
                core->Damage(damageProps);
            }
        }

        _resumeToCleanUp = true;
        return PlayResult::Resume();
    }
    else if (_resumeToCleanUp)
    {
        _resumeToCleanUp = false;

        core->AddCardToHand(core->RemoveCardFromInPlayCards(_cardRapidAttack), actionProps.player);
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardPreparations));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardDragonPower));

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}