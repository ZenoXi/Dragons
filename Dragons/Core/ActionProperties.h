#pragma once

constexpr int DEFAULT_PLAYER = -1;
constexpr int DEFAULT_OPPONENT = -1;
constexpr bool DEFAULT_FREE_ACTION = false;
constexpr bool DEFAULT_PLAY_ON_SELF = false;

struct ActionProperties
{
    // Player performing the action
    int player = DEFAULT_PLAYER;
    // Opponent to the player performing the action
    int opponent = DEFAULT_OPPONENT;
    // Does the action reduce actions left
    bool freeAction = DEFAULT_FREE_ACTION;
    // Play on self (fuck Devils Deal)
    bool playOnSelf = DEFAULT_PLAY_ON_SELF;
};
