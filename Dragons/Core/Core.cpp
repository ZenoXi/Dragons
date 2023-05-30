#include "Core.h"

void Core::InitState()
{
    ClearState();

    // Init decks
    //_state.offenseDeck.push_back(std::make_unique<Cards::Stab>());
    //_state.defenseDeck.push_back(std::make_unique<Cards::Healer>());
    //_state.utilityDeck.push_back(std::make_unique<Cards::TheThief>());
    //_state.comboDeck.push_back(std::make_unique<Cards::Omniclaustra>());

    // Init players
    _state.players.push_back(Player{});
    _state.players[0].health = 10;
    _state.players[0].maxHealth = 10;
    _state.players[0].armor = 0;
    _state.players[0].actionsLeft = 0;
    _state.players[0].extraPlays = 0;
    _state.players[0].extraDraws = 0;

    _state.players.push_back(Player{});
    _state.players[1].health = 10;
    _state.players[1].maxHealth = 10;
    _state.players[1].armor = 0;
    _state.players[1].actionsLeft = 0;
    _state.players[1].extraPlays = 0;
    _state.players[1].extraDraws = 0;

    _state.currentPlayer = 0;
}

void Core::ClearState()
{

}