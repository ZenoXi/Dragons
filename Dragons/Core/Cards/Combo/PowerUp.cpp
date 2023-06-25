#include "PowerUp.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::PowerUp::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::PowerUp::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == ArmorUp::CARD_ID())
            _cardArmorUp = card;
        else if (card->GetCardId() == Healer::CARD_ID())
            _cardHealer = card;
    }

    // Play Armor Up
    ArmorUpPlayProperties armorUpPlayProperties;
    armorUpPlayProperties.armorAmount = 4;
    _cardArmorUp->Play(core, actionProps, &armorUpPlayProperties);

    // Play Healer
    HealerPlayProperties healerPlayProperties;
    healerPlayProperties.healAmount = 4;
    _cardHealer->Play(core, actionProps, &healerPlayProperties);

    core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardArmorUp));
    core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardHealer));
    return PlayResult::Default();
}