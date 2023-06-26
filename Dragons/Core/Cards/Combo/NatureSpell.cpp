#include "NatureSpell.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::NatureSpell::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::NatureSpell::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == Cure::CARD_ID())
            _cardCure = card;
        else if (card->GetCardId() == LifeFlower::CARD_ID())
            _cardLifeFlower = card;
    }

    // Play Cure
    int healAmount = 1;
    _postHealHandler = std::make_unique<EventHandler<PostHealEvent>>(&core->Events(), [=, &healAmount](PostHealEvent event)
    {
        if (event.props.target != actionProps.player)
            return;

        healAmount = event.props.amount;
    });
    _cardCure->Play(core, actionProps, nullptr);
    _postHealHandler.reset();

    // Play Life Flower
    LifeFlowerPlayProperties lifeFlowerPlayProperties;
    lifeFlowerPlayProperties.armorAmount = healAmount;
    lifeFlowerPlayProperties.maxHealthIncrease = healAmount;
    _cardLifeFlower->Play(core, actionProps, &lifeFlowerPlayProperties);

    return PlayResult::Default();
}