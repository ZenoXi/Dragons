#include "TotalEntanglement.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::TotalEntanglement::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::TotalEntanglement::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == FireMoon::CARD_ID())
            _cardFireMoon = card;
        else if (card->GetCardId() == SoothingSpell::CARD_ID())
            _cardSoothingSpell = card;
        else if (card->GetCardId() == AbsoluteZero::CARD_ID())
            _cardAbsoluteZero = card;
    }

    // Play Absolute Zero
    _cardAbsoluteZero->Play(core, actionProps, nullptr);
    core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardAbsoluteZero), actionProps.player);

    // Play Fire Moon
    PlayResult result = _cardFireMoon->Play(core, actionProps, nullptr);
    if (result.waitForInput)
    {
        _resumeFireMoon = true;
        return result;
    }
    else
    {
        core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardFireMoon), actionProps.player);
        _resumeToSoothingSpell = true;
        return PlayResult::Resume();
    }

}

cards::PlayResult cards::TotalEntanglement::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_resumeFireMoon)
    {
        _resumeFireMoon = false;

        core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardFireMoon), actionProps.player);

        _resumeToSoothingSpell = true;
        return PlayResult::Resume();
    }
    else if (_resumeToSoothingSpell)
    {
        _resumeToSoothingSpell = false;

        // Play Soothing Spell
        _cardAbsoluteZero->Play(core, actionProps, nullptr);
        core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardAbsoluteZero), actionProps.player);

        // Discard opponent hand
        auto& handRef = core->GetState().players[actionProps.opponent].hand;
        while (!handRef.empty())
            core->AddCardToGraveyard(core->RemoveCardFromHand(0, actionProps.opponent));

        // Add extra actions
        core->AddExtraAction(actionProps.player, ExtraAction(true, true, false));

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}