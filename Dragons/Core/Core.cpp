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
    _state.players[0].index = 0;

    _state.players.push_back(Player{});
    _state.players[1].health = 10;
    _state.players[1].maxHealth = 10;
    _state.players[1].armor = 0;
    _state.players[1].actionsLeft = 0;
    _state.players[1].extraPlays = 0;
    _state.players[1].extraDraws = 0;
    _state.players[1].index = 1;

    _state.currentPlayer = 0;
    _state.opposingPlayer = 1;
}

void Core::ClearState()
{

}

bool Core::PlayCard(cards::Card* card)
{
    return PlayCard(card, nullptr, nullptr);
}

bool Core::PlayCard(cards::Card* card, cards::ActionProperties* actionProps, cards::PlayProperties* playProps)
{
    cards::ActionProperties actionPropsLocal{};
    if (actionProps)
    {
        actionPropsLocal = *actionProps;
    }
    else
    {
        actionPropsLocal.player = _state.currentPlayer;
        actionPropsLocal.opponent = _state.opposingPlayer;
    }

    auto& hand = _state.players[actionPropsLocal.player].hand;
    auto foundCardIt = std::find_if(hand.begin(), hand.end(), [&](std::unique_ptr<cards::Card>& cardPtr) { return cardPtr.get() == card; });
    if (foundCardIt == hand.end())
        return false;

    // Emit pre play event

    cards::PlayResult result = card->Play(this, actionPropsLocal, playProps);
    if (result.discard)
    {
        _state.graveyard.push_back(std::move(*foundCardIt));
        hand.erase(foundCardIt);
    }

    // Emit post play event

    return true;
}

void Core::Damage(DamageProperties props)
{
    // Emit pre damage event

    Player& targetPlayer = _state.players[props.target];
    if (!props.ignoreArmor)
    {
        if (targetPlayer.armor > 0)
        {
            if (props.amount > targetPlayer.armor)
            {
                props.amount -= targetPlayer.armor;
                targetPlayer.armor = 0;
            }
            else
            {
                targetPlayer.armor -= props.amount;
                props.amount = 0;
            }
        }
    }
    if (props.amount > 0)
    {
        if (props.amount >= targetPlayer.health)
        {
            props.amount -= targetPlayer.health;
            targetPlayer.health = 0;
        }
        else
        {
            targetPlayer.health -= props.amount;
            props.amount = 0;
        }
    }

    // Emit post damage event
}

void Core::Heal(int target, int amount)
{

}

void Core::AddArmor(int target, int amount)
{

}

void Core::DestroyArmor(int target)
{

}

void Core::SetMaxHealth(int target, int amount)
{

}