#pragma once

#include "GameState.h"

struct DamageProperties
{
    int target;
    int amount;
    bool ignoreArmor;
    bool trueDamage;
};

class Core
{
    GameState _state;

public:
    Core() {}

    void InitState();
    void ClearState();
    const GameState& GetState() { return _state; }

    bool PlayCard(cards::Card* card);
    bool PlayCard(cards::Card* card, cards::ActionProperties* actionProps, cards::PlayProperties* playProps);

    void Damage(DamageProperties props);
    void Heal(int target, int amount);
    void AddArmor(int target, int amount);
    void DestroyArmor(int target);
    void SetMaxHealth(int target, int amount);
    
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