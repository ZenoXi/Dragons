#pragma once

#include "Cards/Card.h"

#include <vector>
#include <memory>

struct Player
{
    int health;
    int maxHealth;
    int armor;
    int actionsLeft;
    int extraPlays;
    int extraDraws;
    int extraDiscards;
    std::vector<std::unique_ptr<cards::Card>> hand;
    std::vector<std::unique_ptr<cards::Card>> activeCards;

    bool handRevealed;

    int index;

    int CardsInHand(cards::CardType type) const
    {
        int count = 0;
        for (auto& card : hand)
        {
            if (card->GetCardType() == type)
            {
                count++;
            }
        }
        return count;
    }
};