#include "Superiority.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::Superiority::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::Superiority::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == HellfireSword::CARD_ID())
            _cardHellfireSword = card;
        else if (card->GetCardId() == ArmorUp::CARD_ID())
            _cardArmorUp = card;
        else if (card->GetCardId() == Cheating::CARD_ID())
            _cardCheating = card;
    }

    // Play Hellfire Sword
    HellfireSwordPlayProperties hellfireSwordPlayProperties;
    hellfireSwordPlayProperties.stealArmor = true;
    _cardHellfireSword->Play(core, actionProps, &hellfireSwordPlayProperties);

    // Play Armor Up
    _cardArmorUp->Play(core, actionProps, nullptr);

    core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardHellfireSword));
    core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardArmorUp));
    core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardCheating));

    if (core->GetState().players[actionProps.player].armor > 0)
    {
        return PlayResult::AddToActives();
    }
    else
    {
        return PlayResult::Default();
    }
}

void cards::Superiority::_OnEnterHand(Core* core, int playerIndex)
{
    if (_revealedHandIndex != -1)
        core->HideHand(_revealedHandIndex, "superiority");
}

void cards::Superiority::_OnEnterActiveCards(Core* core, int playerIndex)
{
    if (_revealedHandIndex != -1)
        core->HideHand(_revealedHandIndex, "superiority");

    _revealedHandIndex = core->OpponentOf(playerIndex);
    core->RevealHand(_revealedHandIndex, "superiority");

    _postArmorChangeHandler = std::make_unique<EventHandler<PostArmorChangeEvent>>(&core->Events(), [=](PostArmorChangeEvent event)
    {
        if (GetPosition().playerIndex != event.target)
            return;

        if (event.newValue == 0)
        {
            if (_revealedHandIndex != -1)
                core->HideHand(_revealedHandIndex, "superiority");

            auto cardPtr = core->RemoveCardFromActiveCards(this, GetPosition().playerIndex);
            core->AddCardToGraveyard(std::move(cardPtr));
        }
    });
}

void cards::Superiority::_OnEnterDeck(Core* core)
{
    if (_revealedHandIndex != -1)
        core->HideHand(_revealedHandIndex, "superiority");
}

void cards::Superiority::_OnEnterGraveyard(Core* core)
{
    if (_revealedHandIndex != -1)
        core->HideHand(_revealedHandIndex, "superiority");
}

void cards::Superiority::_OnEnterDestroyedCards(Core* core)
{
    if (_revealedHandIndex != -1)
        core->HideHand(_revealedHandIndex, "superiority");
}