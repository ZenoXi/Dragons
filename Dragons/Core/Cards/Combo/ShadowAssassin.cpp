#include "ShadowAssassin.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::ShadowAssassin::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::ShadowAssassin::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    // TODO: this should be an active card

    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == VitalSpot::CARD_ID())
            _cardVitalSpot = card;
        else if (card->GetCardId() == DeathPoison::CARD_ID())
            _cardDeathPoison = card;
    }

    // Play Vital Spot
    _cardVitalSpot->Play(core, actionProps, nullptr);
    core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardVitalSpot), actionProps.player);

    // Play Death Poison
    _cardDeathPoison->Play(core, actionProps, nullptr);
    core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardDeathPoison), actionProps.player);

    // Start combo effect (this entire portion is just juggling burning knives while walking on a tightrope above a volcano)
    auto effect = std::make_unique<_ComboEffect>();
    effect->preDamageHandler = std::make_unique<EventHandler<PreDamageEvent_BuffPass>>(&core->Events(), [=](PreDamageEvent_BuffPass event)
    {
        if (actionProps.player != event.props->source)
            return;

        event.props->trueDamage = true;
        event.props->ignoreArmor = true;
    }, true);
    effect->turnEndHandler = std::make_unique<EventHandler<TurnEndEvent>>(&core->Events(), [=, effectPtr = effect.get()](TurnEndEvent event)
    {
        for (int i = 0; i < _activeComboEffects.size(); i++)
        {
            if (_activeComboEffects[i].get() == effectPtr)
            {
                _activeComboEffects.erase(_activeComboEffects.begin() + i);
                return;
            }
        }
    });
    _activeComboEffects.push_back(std::move(effect));

    core->AddExtraAction(actionProps.player, { true, false, false });

    return PlayResult::Default();
}