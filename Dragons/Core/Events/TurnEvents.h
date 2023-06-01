#pragma once

struct TurnBeginEvent
{
    static const char* _NAME_() { return "turn_begin"; }
    int playerIndex;
    int opponentIndex;
};

struct TurnEndEvent
{
    static const char* _NAME_() { return "turn_end"; }
    int playerIndex;
    int opponentIndex;
};