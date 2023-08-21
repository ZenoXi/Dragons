#pragma once

#include "GameState.h"
#include "ActionProperties.h"
#include "DamageProperties.h"
#include "HealProperties.h"
#include "AddArmorProperties.h"
#include "ComboProperties.h"
#include "Events/GameEvents.h"

#include <optional>
#include <random>

class Core
{
    GameState _state;
    GameEvents _events;

    int _initialTurnsLeft = 4;

    std::vector<std::unique_ptr<cards::Card>> _registeredCards;

    std::mt19937 _rng;

    // Info necessary to resume after user input
    struct _InPlayCard
    {
        cards::Card* card = nullptr;
        ActionProperties actionProps;
        cards::PlayProperties* playProps = nullptr;
    };
    std::vector<_InPlayCard> _currentlyPlayingCards;

    int _actionsConsumed = 0;
    std::vector<ExtraAction> _extraActionsConsumed;

public:
    Core();

    void InitState(uint32_t seed);
    void ClearState();
    const GameState& GetState() { return _state; }
    GameEvents& Events() { return _events; }

    void EndTurn();

    // Main game actions
    bool CanPlayCard(cards::Card* card);
    bool CanPlayCard(cards::Card* card, std::optional<ActionProperties> actionProps, cards::PlayProperties* playProps);
    cards::PlayResult PlayCard(cards::Card* card);
    cards::PlayResult PlayCard(cards::Card* card, std::optional<ActionProperties> actionProps, cards::PlayProperties* playProps);
    cards::PlayResult ResumePlay(UserInputResponse&& response);
    void MoveCardAfterPlay(const cards::PlayResult& result, cards::Card* playedCard, ActionProperties actionProps, cards::PlayProperties* playProps);
private:
    cards::PlayResult _HandlePlayResult(cards::PlayResult result, cards::Card* playedCard, ActionProperties actionProps, cards::PlayProperties* playProps, bool playIsResume = false);
    
public:
    bool CanDrawCard(cards::CardType deck, int playerIndex);
    cards::Card* DrawCard(cards::CardType type, int playerIndex, bool consumeAction = true);
    bool CanDiscardCard(cards::Card* card, int playerIndex);
    cards::Card* DiscardCard(cards::Card* card, int playerIndex, bool consumeAction = true);

    bool CanPlayComboCard(ComboProperties comboProps);
    bool CanPlayComboCard(ComboProperties comboProps, std::optional<ActionProperties> actionProps, cards::PlayProperties* playProps);
    std::vector<cards::Card*> GetCardsForCombo(ComboProperties comboProps);

    // Stat manipulation
    DamageResult Damage(DamageProperties props);
    void Heal(HealProperties props);
    void AddArmor(AddArmorProperties props);
    void DestroyArmor(int target);
    void SetMaxHealth(int target, int value, bool force = false);
    void SetHealth(int target, int value, bool force = false);
    void SetArmor(int target, int value, bool force = false);

    void SetActionCount(int target, int amount);
    void AddActions(int target, int amount);
    void AddExtraAction(int target, ExtraAction action);

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
    void AddCardToInPlayCards(std::unique_ptr<cards::Card> card);
    std::unique_ptr<cards::Card> RemoveCardFromInPlayCards(cards::Card* card);
    std::unique_ptr<cards::Card> RemoveCardFromInPlayCards(int cardIndex);
    void AddCardToDestroyedCards(std::unique_ptr<cards::Card> card);
    std::unique_ptr<cards::Card> RemoveCardFromDestroyedCards(cards::Card* card);
    std::unique_ptr<cards::Card> RemoveCardFromDestroyedCards(int cardIndex);
    void AddCardToDisplayedCards(DisplayInfo displayInfo);
    bool RemoveCardFromDisplayedCards(cards::Card* card);
    void RemoveCardFromDisplayedCards(int cardIndex);
    bool ModifyDisplayedCard(DisplayInfo newDisplayInfo);
    void ClearDisplayedCards();
    cards::CardSet GetCardSet(cards::Card* card);

    // Other
    void ShuffleDeck(cards::CardType type);
    int GenerateRandomNumber(int fromInclusive, int toExclusive);
    void RevealHand(int target, std::string revealSource = "-");
    void HideHand(int target, std::string revealSource = "-");
    std::unique_ptr<cards::Card> CreateCard(CardId cardId);
    std::vector<cards::Card*> GetRegisteredCards();
    int OpponentOf(int playerIndex);

private:
    std::vector<std::unique_ptr<cards::Card>>& _ResolveDeckFromType(cards::CardType type);
};