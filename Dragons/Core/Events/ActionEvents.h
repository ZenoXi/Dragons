#pragma once

#include "../Cards/Card.h"

struct PreCardPlayedEvent
{
    static const char* _NAME_() { return "pre_card_played"; }
    cards::Card* card;
    ActionProperties* actionProps;
    cards::PlayProperties* playProps;
};

struct PostCardPlayedEvent
{
    static const char* _NAME_() { return "post_card_played"; }
    cards::Card* card;
    ActionProperties* actionProps;
    cards::PlayProperties* playProps;
};