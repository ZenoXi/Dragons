#include "UltimateDefense.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::UltimateDefense::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::UltimateDefense::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == WarBlessing::CARD_ID())
            _cardWarBlessing = card;
        else if (card->GetCardId() == Barrier::CARD_ID())
            _cardBarrier = card;
    }

    // Play War Blessing
    _cardWarBlessing->Play(core, actionProps, nullptr);

    // Play Barrier
    _cardBarrier->Play(core, actionProps, nullptr);
    core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardBarrier), actionProps.player);

    // Draw card
    if (core->GetState().players[actionProps.player].hand.size() < GAME_HAND_SIZE)
        core->DrawCard(CardType::DEFENSE, actionProps.player, false);

    core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardWarBlessing));

    return PlayResult::Default();
}