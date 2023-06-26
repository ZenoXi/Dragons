#include "Soulstealer.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::Soulstealer::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::Soulstealer::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == Stab::CARD_ID())
            _cardStab = card;
        else if (card->GetCardId() == Cure::CARD_ID())
            _cardCure = card;
        else if (card->GetCardId() == TheThief::CARD_ID())
            _cardTheThief = card;
    }

    // Play The Thief
    PlayResult result = _cardTheThief->Play(core, actionProps, nullptr);
    if (result.waitForInput)
    {
        _resumeTheThief = true;
        return result;
    }

    _resumeToHealthSteal = true;
    return PlayResult::Resume();
}

cards::PlayResult cards::Soulstealer::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_resumeTheThief)
    {
        _resumeTheThief = false;

        PlayResult result = _cardTheThief->Resume(std::move(response), core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeTheThief = true;
            return result;
        }

        _resumeToHealthSteal = true;
        return PlayResult::Resume();
    }
    else if (_resumeToHealthSteal)
    {
        _resumeToHealthSteal = false;

        int healAmount = 0;
        _postHealHandler = std::make_unique<EventHandler<PostHealEvent>>(&core->Events(), [=, &healAmount](PostHealEvent event)
        {
            if (event.props.target != actionProps.player)
                return;

            healAmount = event.props.amount;
        });
        _cardCure->Play(core, actionProps, nullptr);
        _postHealHandler.reset();

        if (healAmount == 0)
        {
            _resumeToCleanUp = true;
            return PlayResult::Resume();
        }

        DamageProperties damageProps;
        damageProps.source = actionProps.player;
        damageProps.target = actionProps.opponent;
        damageProps.amount = healAmount;
        damageProps.ignoreArmor = true;
        damageProps.trueDamage = true;
        core->Damage(damageProps);

        _resumeToCleanUp = true;
        return PlayResult::Resume();
    }
    else if (_resumeToCleanUp)
    {
        _resumeToCleanUp = false;

        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardStab));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardCure));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardTheThief));

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}