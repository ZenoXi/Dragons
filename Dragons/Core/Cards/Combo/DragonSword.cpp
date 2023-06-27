#include "DragonSword.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::DragonSword::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::DragonSword::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == HellfireSword::CARD_ID())
            _cardHellfireSword = card;
        else if (card->GetCardId() == HeavySlash::CARD_ID())
            _cardHeavySlash = card;
    }

    // Play Hellfire Sword
    _cardHellfireSword->Play(core, actionProps, nullptr);

    // Play Heavy Slash
    _cardHeavySlash->Play(core, actionProps, nullptr);
    core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardHeavySlash), actionProps.player);

    return PlayResult::AddToActives();
}

void cards::DragonSword::_OnEnterHand(Core* core, int playerIndex)
{
    _turnBeginHandler.reset();
}

void cards::DragonSword::_OnEnterActiveCards(Core* core, int playerIndex)
{
    _turnBeginHandler = std::make_unique<EventHandler<TurnBeginEvent>>(&core->Events(), [=](TurnBeginEvent event)
    {
        if (GetPosition().playerIndex != event.playerIndex)
            return;

        core->DestroyArmor(event.opponentIndex);
        core->AddCardToGraveyard(core->RemoveCardFromActiveCards(this, GetPosition().playerIndex));
    }, true);
}

void cards::DragonSword::_OnEnterDeck(Core* core)
{
    _turnBeginHandler.reset();
}

void cards::DragonSword::_OnEnterGraveyard(Core* core)
{
    _turnBeginHandler.reset();
}

void cards::DragonSword::_OnEnterDestroyedCards(Core* core)
{
    _turnBeginHandler.reset();
}