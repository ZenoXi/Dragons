#pragma once

#include "GameState.h"

class Core
{
    GameState _state;

public:
    Core() {}

    void InitState();
    void ClearState();
    const GameState& GetState() { return _state; }

    // Core class contains game state and methods for interacting with said game state
    // State:
    //  - Player array:
    //    - Stats
    //    - Hand
    //    - Active cards
    //  - Current turn
    //  - Decks
    //  - Graveyard
    //  - Current action
    //


};