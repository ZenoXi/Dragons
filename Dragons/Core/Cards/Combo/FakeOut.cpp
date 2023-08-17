#include "FakeOut.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::FakeOut::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::FakeOut::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == PeacePact::CARD_ID())
            _cardPeacePact = card;
        else if (card->GetCardId() == Preparations::CARD_ID())
            _cardPreparations = card;
    }

    // Play Praparations
    _cardPreparations->Play(core, actionProps, nullptr);

    // Play Divine Protection
    PeacePactPlayProperties peacePactPlayProperties;
    peacePactPlayProperties.discardSelf = false;
    _cardPeacePact->Play(core, actionProps, &peacePactPlayProperties);

    core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardPeacePact));
    core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardPreparations));

    return PlayResult::AddToActives();
}

void cards::FakeOut::_OnEnterHand(Core* core, int playerIndex)
{
    _turnBeginHandler.reset();
    _turnEndHandler.reset();
    _canDrawHandler.reset();
    _activated = false;
}

void cards::FakeOut::_OnEnterActiveCards(Core* core, int playerIndex)
{
    _activated = false;

    _turnBeginHandler = std::make_unique<EventHandler<TurnBeginEvent>>(&core->Events(), [=](TurnBeginEvent event)
    {
        if (event.opponentIndex != GetPosition().playerIndex)
            return;

        _activated = true;
    });
    _turnEndHandler = std::make_unique<EventHandler<TurnEndEvent>>(&core->Events(), [=](TurnEndEvent event)
    {
        if (!_activated)
            return;

        auto cardPtr = core->RemoveCardFromActiveCards(this, GetPosition().playerIndex);
        core->AddCardToGraveyard(std::move(cardPtr));
    });
    _canDrawHandler = std::make_unique<EventHandler<CanDrawEvent>>(&core->Events(), [=](CanDrawEvent event)
    {
        if (GetPosition().playerIndex == event.playerIndex)
            return;

        if (event.deck == CardType::OFFENSE)
            *event.canDraw = false;
    });
}

void cards::FakeOut::_OnEnterDeck(Core* core)
{
    _turnBeginHandler.reset();
    _turnEndHandler.reset();
    _canDrawHandler.reset();
    _activated = false;
}

void cards::FakeOut::_OnEnterGraveyard(Core* core)
{
    _turnBeginHandler.reset();
    _turnEndHandler.reset();
    _canDrawHandler.reset();
    _activated = false;
}

void cards::FakeOut::_OnEnterDestroyedCards(Core* core)
{
    _turnBeginHandler.reset();
    _turnEndHandler.reset();
    _canDrawHandler.reset();
    _activated = false;
}