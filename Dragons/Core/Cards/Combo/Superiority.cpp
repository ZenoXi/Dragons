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
        core->RevealHand(actionProps.opponent, "superiority");
        return PlayResult::AddToActives();
    }
    else
    {
        return PlayResult::Default();
    }
}

void cards::Superiority::_OnEnterHand(Core* core, int playerIndex)
{
    core->HideHand(_opponentIndex, "superiority");
}

void cards::Superiority::_OnEnterActiveCards(Core* core, int playerIndex)
{
    _postArmorChangeHandler = std::make_unique<EventHandler<PostArmorChangeEvent>>(&core->Events(), [=](PostArmorChangeEvent event)
    {
        if (GetPosition().playerIndex != event.target)
            return;

        if (event.newValue == 0)
        {
            core->HideHand(_opponentIndex, "superiority");
            auto cardPtr = core->RemoveCardFromActiveCards(this, GetPosition().playerIndex);
            core->AddCardToGraveyard(std::move(cardPtr));
        }
    });
}

void cards::Superiority::_OnEnterDeck(Core* core)
{
    core->HideHand(_opponentIndex, "superiority");
}

void cards::Superiority::_OnEnterGraveyard(Core* core)
{
    core->HideHand(_opponentIndex, "superiority");
}

void cards::Superiority::_OnEnterDestroyedCards(Core* core)
{
    core->HideHand(_opponentIndex, "superiority");
}