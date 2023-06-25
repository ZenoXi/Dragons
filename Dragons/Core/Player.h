#pragma once

#include "Cards/Card.h"

#include <vector>
#include <memory>

struct ExtraAction
{
    bool play;
    bool draw;
    bool discard;

    ExtraAction(bool play, bool draw, bool discard) : play(play), draw(draw), discard(discard) {}

    // The weight of an extra action shows how valuable the extra action is
    // When deciding which extra action to use, the least valuable available is used
    int Weight() const
    {
        int weight = 0;
        if (play)
            weight += 3;
        if (draw)
            weight += 2;
        if (discard)
            weight += 1;
    }
};

struct Player
{
    int health;
    int maxHealth;
    int armor;
    int actionsLeft;
    std::vector<ExtraAction> extraActions;
    std::vector<std::unique_ptr<cards::Card>> hand;
    std::vector<std::unique_ptr<cards::Card>> activeCards;

    bool handRevealed;
    std::vector<std::string> revealSources;

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