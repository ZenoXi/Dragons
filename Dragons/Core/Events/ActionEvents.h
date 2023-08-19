#pragma once

#include "../Cards/Card.h"
#include "../ComboProperties.h"
#include "../Player.h"

struct CanPlayEvent
{
    static const char* _NAME_() { return "can_play"; }
    cards::Card* card;
    ActionProperties* actionProps;
    cards::PlayProperties* playProps;
    bool* canPlay;
};

struct PreCardPlayedEvent
{
    static const char* _NAME_() { return "pre_card_played"; }
    cards::Card* card;
    ActionProperties* actionProps;
    cards::PlayProperties** playProps;
};

struct PostCardPlayedEvent
{
    static const char* _NAME_() { return "post_card_played"; }
    cards::Card* card;
    ActionProperties* actionProps;
    cards::PlayProperties* playProps;
};

struct CanPlayComboEvent
{
    static const char* _NAME_() { return "can_play_combo"; }
    ComboProperties comboProps;
    ActionProperties* actionProps;
    cards::PlayProperties* playProps;
    bool* canPlay;
};

struct PreGetComboCardsEvent
{
    static const char* _NAME_() { return "pre_get_combo_cards"; }
    ComboProperties comboProps;
    std::vector<cards::Card*>* retrievedCards;
    bool* processed;
};

struct PostGetComboCardsEvent
{
    static const char* _NAME_() { return "post_get_combo_cards"; }
    ComboProperties comboProps;
    std::vector<cards::Card*>* retrievedCards;
};

struct CanDrawEvent
{
    static const char* _NAME_() { return "can_draw"; }
    int playerIndex;
    cards::CardType deck;
    bool* canDraw;
};

struct ActionCountChangedEvent
{
    static const char* _NAME_() { return "action_count_changed"; }
    int playerIndex;
    int newActionsLeft;
    std::vector<ExtraAction> newExtraActions;
};