#include "DreamLand.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::DreamLand::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::DreamLand::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == VitalSpot::CARD_ID())
            _cardVitalSpot = card;
        else if (card->GetCardId() == SpiritShelter::CARD_ID())
            _cardSpiritShelter = card;
        else if (card->GetCardId() == SetUp::CARD_ID())
            _cardSetUp = card;
    }

    // Play Set Up
    SetUpPlayProperties setUpPlayProperties;
    setUpPlayProperties.actionCount = 6;
    _cardSetUp->Play(core, actionProps, &setUpPlayProperties);
    core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardSetUp), actionProps.player);

    // Play Vital Spot
    VitalSpotPlayProperties vitalSpotPlayProperties;
    vitalSpotPlayProperties.attacksToEnchance = 2;
    _cardVitalSpot->Play(core, actionProps, &vitalSpotPlayProperties);
    core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardVitalSpot), actionProps.player);

    // Play Spirit Shelter
    _cardSpiritShelter->Play(core, actionProps, nullptr);
    core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardSpiritShelter), actionProps.player);

    return PlayResult::Default();
}