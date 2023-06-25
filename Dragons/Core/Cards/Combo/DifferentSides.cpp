#include "DifferentSides.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::DifferentSides::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::DifferentSides::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == DesperateStrike::CARD_ID())
            _cardDesperateStrike = card;
        else if (card->GetCardId() == Healer::CARD_ID())
            _cardHealer = card;
        else if (card->GetCardId() == HiddenTreasures::CARD_ID())
            _cardHiddenTreasures = card;
    }

    // Play Desperate Strike
    DesperateStrikePlayProperties desperateStrikePlayProperties;
    desperateStrikePlayProperties.damageAmount = 5;
    _cardDesperateStrike->Play(core, actionProps, &desperateStrikePlayProperties);

    // Play Hidden Treasures
    if (_cardHiddenTreasures->CanPlay(core, actionProps, nullptr))
    {
        PlayResult result = _cardHiddenTreasures->Play(core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeHiddenTreasures = true;
            return result;
        }
    }

    _resumeToHealer = true;
    return PlayResult::Resume();
}

cards::PlayResult cards::DifferentSides::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_resumeHiddenTreasures)
    {
        _resumeHiddenTreasures = false;

        PlayResult result = _cardHiddenTreasures->Resume(std::move(response), core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeHiddenTreasures = true;
            return result;
        }

        _resumeToHealer = true;
        return PlayResult::Resume();
    }
    else if (_resumeToHealer)
    {
        _resumeToHealer = false;

        HealerPlayProperties healerPlayProperties;
        if (core->GetState().players[actionProps.player].hand.size() > core->GetState().players[actionProps.opponent].hand.size())
            healerPlayProperties.healAmount = 5;
        _cardHealer->Play(core, actionProps, &healerPlayProperties);

        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardDesperateStrike));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardHealer));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardHiddenTreasures));

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}