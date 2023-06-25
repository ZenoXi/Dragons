#include "Omniclaustra.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::Omniclaustra::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::Omniclaustra::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == SoothingSpell::CARD_ID())
            _cardSoothingSpell = card;
        else if (card->GetCardId() == DivineProtection::CARD_ID())
            _cardDivineProtection = card;
    }

    // Play Soothing Spell
    _cardSoothingSpell->Play(core, actionProps, nullptr);
    core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardSoothingSpell), actionProps.player);

    // Play Divine Protection
    _cardDivineProtection->Play(core, actionProps, nullptr);
    core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardDivineProtection), actionProps.player);

    return PlayResult::AddToActives();
}

void cards::Omniclaustra::_OnEnterHand(Core* core, int playerIndex)
{
    _preHealthChangeHandler.reset();
}

void cards::Omniclaustra::_OnEnterActiveCards(Core* core, int playerIndex)
{
    _preHealthChangeHandler = std::make_unique<EventHandler<PreHealthChangeEvent>>(&core->Events(), [=](PreHealthChangeEvent event)
    {
        if (GetPosition().playerIndex != event.target)
            return;

        if (*event.newValue < 1 && event.sourceCard && event.sourceCard->GetCardType() == CardType::OFFENSE)
            *event.newValue = 1;
    });
}

void cards::Omniclaustra::_OnEnterDeck(Core* core)
{
    _preHealthChangeHandler.reset();
}

void cards::Omniclaustra::_OnEnterGraveyard(Core* core)
{
    _preHealthChangeHandler.reset();
}

void cards::Omniclaustra::_OnEnterDestroyedCards(Core* core)
{
    _preHealthChangeHandler.reset();
}