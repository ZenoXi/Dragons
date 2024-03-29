#include "CorpsePuppet.h"

#include "../../Core.h"

cards::PlayResult cards::CorpsePuppet::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return PlayResult::AddToActives();
}

void cards::CorpsePuppet::_OnEnterHand(Core* core, int playerIndex)
{
    _canPlayComboHandler.reset();
    _preGetComboCardsHandler.reset();
}

void cards::CorpsePuppet::_OnEnterActiveCards(Core* core, int playerIndex)
{
    _canPlayComboHandler = std::make_unique<EventHandler<CanPlayComboEvent>>(&core->Events(), [=](CanPlayComboEvent event)
    {
        if (GetPosition().playerIndex != event.comboProps.player)
            return;
        if (*event.canPlay)
            return;

        bool canPlay = true;
        bool graveyardUsed = false;
        for (auto& requiredCardId : event.comboProps.requiredCards)
        {
            bool cardFound = false;
            if (!cardFound)
            {
                for (auto& card : core->GetState().players[event.comboProps.player].hand)
                {
                    if (card->GetCardId() == requiredCardId)
                    {
                        cardFound = true;
                        break;
                    }
                }
            }
            if (!cardFound && !graveyardUsed)
            {
                for (auto& card : core->GetState().graveyard)
                {
                    if (card->GetCardId() == requiredCardId)
                    {
                        cardFound = true;
                        graveyardUsed = true;
                        break;
                    }
                }
            }
            if (!cardFound)
            {
                canPlay = false;
                break;
            }
        }
        *event.canPlay = canPlay;
    });
    
    _preGetComboCardsHandler = std::make_unique<EventHandler<PreGetComboCardsEvent>>(&core->Events(), [=](PreGetComboCardsEvent event)
    {
        if (GetPosition().playerIndex != event.comboProps.player)
            return;
        if (*event.processed)
            return;

        bool graveyardUsed = false;
        for (auto& requiredCardId : event.comboProps.requiredCards)
        {
            bool cardFound = false;
            if (!cardFound)
            {
                auto& hand = core->GetState().players[event.comboProps.player].hand;
                for (int i = 0; i < hand.size(); i++)
                {
                    if (hand[i]->GetCardId() == requiredCardId)
                    {
                        auto cardPtr = core->RemoveCardFromHand(i, event.comboProps.player);
                        event.retrievedCards->push_back(cardPtr.get());
                        core->AddCardToInPlayCards(std::move(cardPtr));
                        cardFound = true;
                        break;
                    }
                }
            }
            if (!cardFound && !graveyardUsed)
            {
                auto& graveyard = core->GetState().graveyard;
                for (int i = 0; i < graveyard.size(); i++)
                {
                    if (graveyard[i]->GetCardId() == requiredCardId)
                    {
                        auto cardPtr = core->RemoveCardFromGraveyard(i);
                        event.retrievedCards->push_back(cardPtr.get());
                        core->AddCardToInPlayCards(std::move(cardPtr));
                        cardFound = true;
                        graveyardUsed = true;
                        break;
                    }
                }
            }
        }

        if (graveyardUsed)
        {
            auto cardPtr = core->RemoveCardFromActiveCards(this, GetPosition().playerIndex);
            core->AddCardToGraveyard(std::move(cardPtr));
            *event.processed = true;
        }
    });
}

void cards::CorpsePuppet::_OnEnterDeck(Core* core)
{
    _canPlayComboHandler.reset();
    _preGetComboCardsHandler.reset();
}

void cards::CorpsePuppet::_OnEnterGraveyard(Core* core)
{
    _canPlayComboHandler.reset();
    _preGetComboCardsHandler.reset();
}

void cards::CorpsePuppet::_OnEnterDestroyedCards(Core* core)
{
    _canPlayComboHandler.reset();
    _preGetComboCardsHandler.reset();
}