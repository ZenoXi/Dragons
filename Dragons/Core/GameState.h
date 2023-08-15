#pragma once

#include "Cards/Card.h"
#include "Player.h"

#include <vector>
#include <array>
#include <memory>

struct DisplayInfo
{
    cards::Card* card;
    std::array<bool, 2> visibleTo; // If the i'th player can see the card, visibleTo[i] is true
};

struct GameState
{
    std::vector<Player> players;
    int currentPlayer = -1;
    int opposingPlayer = -1;

    std::vector<std::unique_ptr<cards::Card>> offenseDeck;
    std::vector<std::unique_ptr<cards::Card>> defenseDeck;
    std::vector<std::unique_ptr<cards::Card>> utilityDeck;
    std::vector<std::unique_ptr<cards::Card>> comboDeck;
    std::vector<std::unique_ptr<cards::Card>> graveyard;
    std::vector<std::unique_ptr<cards::Card>> inPlayCards;
    std::vector<std::unique_ptr<cards::Card>> destroyedCards;

    std::vector<DisplayInfo> displayedCards;

    std::vector<std::unique_ptr<cards::Card>>& GetDeck(cards::CardType type)
    {
        switch (type)
        {
        case cards::CardType::OFFENSE:
            return offenseDeck;
        case cards::CardType::DEFENSE:
            return defenseDeck;
        case cards::CardType::UTILITY:
            return utilityDeck;
        case cards::CardType::COMBO:
            return comboDeck;
        }
    }
    const std::vector<std::unique_ptr<cards::Card>>& GetDeck(cards::CardType type) const
    {
        switch (type)
        {
        case cards::CardType::OFFENSE:
            return offenseDeck;
        case cards::CardType::DEFENSE:
            return defenseDeck;
        case cards::CardType::UTILITY:
            return utilityDeck;
        case cards::CardType::COMBO:
            return comboDeck;
        default:
            return offenseDeck;
        }
    }
};