#include "DoubleStrike.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::DoubleStrike::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::DoubleStrike::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == SacrificialAttack::CARD_ID())
            _cardSacrificialAttack = card;
        else if (card->GetCardId() == Stab::CARD_ID())
            _cardStab = card;
    }

    // Play Sacrificial Attack
    SacrificialAttackPlayProperties sacrificialAttackPlayProperties;
    sacrificialAttackPlayProperties.damageSelf = false;
    _cardSacrificialAttack->Play(core, actionProps, &sacrificialAttackPlayProperties);

    // Play Stab
    _cardStab->Play(core, actionProps, nullptr);

    core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardSacrificialAttack));
    core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardStab));
    return PlayResult::Default();
}