#pragma once

#include "../Cards/Card.h"

struct CardEnterHandEvent
{
    static const char* _NAME_() { return "card_enter_hand"; }
    cards::Card* card;
    int playerIndex;
};

struct CardLeaveHandEvent
{
    static const char* _NAME_() { return "card_leave_hand"; }
    cards::Card* card;
    int playerIndex;
};

struct CardEnterActivesEvent
{
    static const char* _NAME_() { return "card_enter_actives"; }
    cards::Card* card;
    int playerIndex;
};

struct CardLeaveActivesEvent
{
    static const char* _NAME_() { return "card_leave_actives"; }
    cards::Card* card;
    int playerIndex;
};

struct CardEnterDeckEvent
{
    static const char* _NAME_() { return "card_enter_deck"; }
    cards::Card* card;
    cards::CardType deck;
};

struct CardLeaveDeckEvent
{
    static const char* _NAME_() { return "card_leave_deck"; }
    cards::Card* card;
    cards::CardType deck;
};

struct CardEnterGraveyardEvent
{
    static const char* _NAME_() { return "card_enter_graveyard"; }
    cards::Card* card;
};

struct CardLeaveGraveyardEvent
{
    static const char* _NAME_() { return "card_leave_graveyard"; }
    cards::Card* card;
};

struct CardEnterDestroyedCardsEvent
{
    static const char* _NAME_() { return "card_enter_destroyed_cards"; }
    cards::Card* card;
};

struct CardLeaveDestroyedCardsEvent
{
    static const char* _NAME_() { return "card_leave_destroyed_cards"; }
    cards::Card* card;
};

struct CardEnterDisplayedCardsEvent
{
    static const char* _NAME_() { return "card_enter_displayed_cards"; }
    cards::Card* card;
};

struct CardLeaveDisplayedCardsEvent
{
    static const char* _NAME_() { return "card_leave_displayed_cards"; }
    cards::Card* card;
};

struct HandRevealStateChangedEvent
{
    static const char* _NAME_() { return "hand_reveal_state_changed"; }
    int playerIndex;
    bool isRevealed;
};