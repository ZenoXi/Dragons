#include "FinalHour.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::FinalHour::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::FinalHour::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == RapidAttack::CARD_ID())
            _cardRapidAttack = card;
        else if (card->GetCardId() == LastBreath::CARD_ID())
            _cardLastBreath = card;
    }

    // Play Rapid Attack
    _cardRapidAttack->Play(core, actionProps, nullptr);

    if (core->GetState().players[actionProps.player].CardsInHand(CardType::OFFENSE) > 0)
    {
        // Play Last Breath
        LastBreathPlayProperties lastBreathPlayProperties;
        lastBreathPlayProperties.discardHand = false;
        PlayResult result = _cardLastBreath->Play(core, actionProps, &lastBreathPlayProperties);
        if (result.waitForInput)
        {
            _resumeLastBreath = true;
            return result;
        }
    }

    _resumeToCleanUp = true;
    return PlayResult::Resume();
}

cards::PlayResult cards::FinalHour::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_resumeLastBreath)
    {
        _resumeLastBreath = false;

        LastBreathPlayProperties lastBreathPlayProperties;
        lastBreathPlayProperties.discardHand = false;
        PlayResult result = _cardLastBreath->Play(core, actionProps, &lastBreathPlayProperties);
        if (result.waitForInput)
        {
            _resumeLastBreath = true;
            return result;
        }

        _resumeToCleanUp = true;
        return PlayResult::Resume();
    }
    else if (_resumeToCleanUp)
    {
        _resumeToCleanUp = false;

        if (core->GetState().players[actionProps.player].hand.empty())
        {
            core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardRapidAttack));
            core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardLastBreath));
            core->SetHealth(actionProps.opponent, 0, true);
        }
        else
        {
            core->AddCardToHand(core->RemoveCardFromInPlayCards(_cardRapidAttack), actionProps.player);
            core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardLastBreath));
        }

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}