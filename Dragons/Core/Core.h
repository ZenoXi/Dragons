#pragma once

#include "GameState.h"
#include "ActionProperties.h"
#include "DamageProperties.h"
#include "Events/GameEvents.h"

#include <optional>
#include <random>

class Core
{
    GameState _state;
    GameEvents _events;

    std::mt19937 _rng;

public:
    Core();

    void InitState();
    void ClearState();
    const GameState& GetState() { return _state; }
    GameEvents& Events() { return _events; }

    // Main game actions
    bool CanPlayCard(cards::Card* card);
    bool CanPlayCard(cards::Card* card, std::optional<ActionProperties> actionProps, cards::PlayProperties* playProps);
    cards::PlayResult PlayCard(cards::Card* card);
    cards::PlayResult PlayCard(cards::Card* card, std::optional<ActionProperties> actionProps, cards::PlayProperties* playProps);
    cards::Card* DrawCard(cards::CardType type, int playerIndex);
    cards::Card* DiscardCard(cards::Card* card, int playerIndex);

    // Stat manipulation
    DamageResult Damage(DamageProperties props);
    void Heal(int target, int amount);
    void AddArmor(int target, int amount);
    void DestroyArmor(int target);
    void SetMaxHealth(int target, int amount);

    void AddExtraActions(int target, int amount);
    void AddExtraPlays(int target, int amount);
    void AddExtraDraws(int target, int amount);
    void AddExtraDiscards(int target, int amount);

    // Card moving functions
    void AddCardToHand(std::unique_ptr<cards::Card> card, int playerIndex);
    std::unique_ptr<cards::Card> RemoveCardFromHand(cards::Card* card, int playerIndex);
    std::unique_ptr<cards::Card> RemoveCardFromHand(int cardIndex, int playerIndex);
    void AddCardToActiveCards(std::unique_ptr<cards::Card> card, int playerIndex);
    std::unique_ptr<cards::Card> RemoveCardFromActiveCards(cards::Card* card, int playerIndex);
    std::unique_ptr<cards::Card> RemoveCardFromActiveCards(int cardIndex, int playerIndex);
    bool AddCardToDeck(std::unique_ptr<cards::Card> card);
    std::unique_ptr<cards::Card> RemoveCardFromDeck(cards::Card* card);
    std::unique_ptr<cards::Card> RemoveCardFromDeck(cards::CardType deck, int cardIndex);
    void AddCardToGraveyard(std::unique_ptr<cards::Card> card);
    std::unique_ptr<cards::Card> RemoveCardFromGraveyard(cards::Card* card);
    std::unique_ptr<cards::Card> RemoveCardFromGraveyard(int cardIndex);
    void AddCardToDestroyedCards(std::unique_ptr<cards::Card> card);
    std::unique_ptr<cards::Card> RemoveCardFromDestroyedCards(cards::Card* card);
    std::unique_ptr<cards::Card> RemoveCardFromDestroyedCards(int cardIndex);
    void AddCardToDisplayedCards(DisplayInfo displayInfo);
    bool RemoveCardFromDisplayedCards(cards::Card* card);
    void RemoveCardFromDisplayedCards(int cardIndex);
    bool ModifyDisplayedCard(DisplayInfo newDisplayInfo);
    void ClearDisplayedCards();

    // Other
    void ShuffleDeck(cards::CardType type);
    int GenerateRandomNumber(int fromInclusive, int toExclusive);
    void RevealHand(int target);
    void HideHand(int target);

private:
    std::vector<std::unique_ptr<cards::Card>>& _ResolveDeckFromType(cards::CardType type);
};