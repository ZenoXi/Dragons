#pragma once

#include "GameState.h"
#include "ActionProperties.h"
#include "DamageProperties.h"
#include "Events/GameEvents.h"

#include <optional>

class Core
{
    GameState _state;
    GameEvents _events;

public:
    Core() {}

    void InitState();
    void ClearState();
    const GameState& GetState() { return _state; }
    GameEvents& Events() { return _events; }

    // Main game actions
    bool CanPlayCard(cards::Card* card);
    bool CanPlayCard(cards::Card* card, std::optional<ActionProperties> actionProps, cards::PlayProperties* playProps);
    cards::PlayResult PlayCard(cards::Card* card);
    cards::PlayResult PlayCard(cards::Card* card, std::optional<ActionProperties> actionProps, cards::PlayProperties* playProps);
    bool DrawCard(cards::CardType type, int playerIndex);
    bool DiscardCard(cards::Card* card, int playerIndex);

    // Stat manipulation
    DamageResult Damage(DamageProperties props);
    void Heal(int target, int amount);
    void AddArmor(int target, int amount);
    void DestroyArmor(int target);
    void SetMaxHealth(int target, int amount);

    // Card moving functions
    void AddCardToHand(std::unique_ptr<cards::Card> card, int playerIndex);
    std::unique_ptr<cards::Card> RemoveCardFromHand(cards::Card* card, int playerIndex);
    void AddCardToActiveCards(std::unique_ptr<cards::Card> card, int playerIndex);
    std::unique_ptr<cards::Card> RemoveCardFromActiveCards(cards::Card* card, int playerIndex);
    bool AddCardToDeck(std::unique_ptr<cards::Card> card);
    std::unique_ptr<cards::Card> RemoveCardFromDeck(cards::Card* card);
    void AddCardToGraveyard(std::unique_ptr<cards::Card> card);
    std::unique_ptr<cards::Card> RemoveCardFromGraveyard(cards::Card* card);
};