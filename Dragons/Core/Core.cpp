#include "Core.h"

#include "GameConstants.h"
#include "Events/TurnEvents.h"
#include "Events/ActionEvents.h"
#include "Events/StatsEvents.h"

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
    _state.players[0].health = GAME_STARTING_HEALTH;
    _state.players[0].maxHealth = GAME_STARTING_MAX_HEALTH;
    _state.players[0].armor = GAME_STARTING_ARMOR;
    _state.players[0].actionsLeft = 0;
    _state.players[0].extraPlays = 0;
    _state.players[0].extraDraws = 0;
    _state.players[0].index = 0;

    _state.players.push_back(Player{});
    _state.players[1].health = GAME_STARTING_HEALTH;
    _state.players[1].maxHealth = GAME_STARTING_MAX_HEALTH;
    _state.players[1].armor = GAME_STARTING_ARMOR;
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

bool Core::CanPlayCard(cards::Card* card)
{
    return CanPlayCard(card, std::nullopt, nullptr);
}

bool Core::CanPlayCard(cards::Card* card, std::optional<ActionProperties> actionProps, cards::PlayProperties* playProps)
{
    ActionProperties actionPropsLocal;
    if (actionProps)
        actionPropsLocal = actionProps.value();

    // Populate default player/opponent values
    if (actionPropsLocal.player == DEFAULT_PLAYER)
        actionPropsLocal.player = _state.currentPlayer;
    if (actionPropsLocal.opponent == DEFAULT_OPPONENT)
        actionPropsLocal.opponent = _state.opposingPlayer;

    bool canPlay = card->CanPlay(this, actionPropsLocal, playProps);

    // Emit event
    CanPlayEvent canPlayEvent;
    canPlayEvent.card = card;
    canPlayEvent.actionProps = &actionPropsLocal;
    canPlayEvent.playProps = playProps;
    canPlayEvent.canPlay = &canPlay;
    _events.RaiseEvent(canPlayEvent);
}

cards::PlayResult Core::PlayCard(cards::Card* card)
{
    return PlayCard(card, std::nullopt, nullptr);
}

cards::PlayResult Core::PlayCard(cards::Card* card, std::optional<ActionProperties> actionProps, cards::PlayProperties* playProps)
{
    ActionProperties actionPropsLocal;
    if (actionProps)
        actionPropsLocal = actionProps.value();

    // Populate default player/opponent values
    if (actionPropsLocal.player == DEFAULT_PLAYER)
        actionPropsLocal.player = _state.currentPlayer;
    if (actionPropsLocal.opponent == DEFAULT_OPPONENT)
        actionPropsLocal.opponent = _state.opposingPlayer;

    auto& hand = _state.players[actionPropsLocal.player].hand;
    auto foundCardIt = std::find_if(hand.begin(), hand.end(), [&](std::unique_ptr<cards::Card>& cardPtr) { return cardPtr.get() == card; });
    if (foundCardIt == hand.end())
    {
        cards::PlayResult result;
        result.notPlayed = true;
        return result;
    }

    // Emit pre play event
    PreCardPlayedEvent preCardPlayedEvent;
    preCardPlayedEvent.card = card;
    preCardPlayedEvent.actionProps = &actionPropsLocal;
    preCardPlayedEvent.playProps = playProps;
    _events.RaiseEvent(preCardPlayedEvent);

    cards::PlayResult result = card->Play(this, actionPropsLocal, playProps);
    if (result.discard)
    {
        _state.graveyard.push_back(std::move(*foundCardIt));
        hand.erase(foundCardIt);
    }
    if (!result.notPlayed)
    {
        // Emit post play event
        PostCardPlayedEvent postCardPlayedEvent;
        postCardPlayedEvent.card = card;
        postCardPlayedEvent.actionProps = &actionPropsLocal;
        postCardPlayedEvent.playProps = playProps;
        _events.RaiseEvent(postCardPlayedEvent);
    }

    return result;
}

DamageResult Core::Damage(DamageProperties props)
{
    PreDamageEvent preDamageEvent;
    preDamageEvent.props = &props;
    _events.RaiseEvent(preDamageEvent);

    DamageResult result;

    Player& targetPlayer = _state.players[props.target];
    if (!props.ignoreArmor)
    {
        if (targetPlayer.armor > 0)
        {
            if (props.amount > targetPlayer.armor)
            {
                props.amount -= targetPlayer.armor;
                result.removedArmorAmount += targetPlayer.armor;
                targetPlayer.armor = 0;
            }
            else
            {
                targetPlayer.armor -= props.amount;
                result.removedArmorAmount += props.amount;
                props.amount = 0;
            }
        }
    }
    if (props.amount > 0)
    {
        if (props.amount >= targetPlayer.health)
        {
            props.amount -= targetPlayer.health;
            result.removedHealthAmount += targetPlayer.health;
            targetPlayer.health = 0;
        }
        else
        {
            targetPlayer.health -= props.amount;
            result.removedHealthAmount += props.amount;
            props.amount = 0;
        }
    }

    return result;
}

void Core::Heal(int target, int amount)
{
    PreHealEvent preHealEvent;
    preHealEvent.target = &target;
    preHealEvent.amount = &amount;
    _events.RaiseEvent(preHealEvent);

    if (amount > 0)
    {
        _state.players[target].armor += amount;
    }
}

void Core::AddArmor(int target, int amount)
{
    PreAddArmorEvent preAddArmorEvent;
    preAddArmorEvent.target = &target;
    preAddArmorEvent.amount = &amount;
    _events.RaiseEvent(preAddArmorEvent);

    if (amount > 0)
    {
        _state.players[target].armor += amount;
    }
}

void Core::DestroyArmor(int target)
{
    bool cancel = false;
    PreDestroyArmorEvent preDestroyArmorEvent;
    preDestroyArmorEvent.target = &target;
    preDestroyArmorEvent.cancel = &cancel;
    _events.RaiseEvent(preDestroyArmorEvent);

    _state.players[target].armor = 0;
}

void Core::SetMaxHealth(int target, int value)
{
    PreSetMaxHealthEvent preSetMaxHealthEvent;
    preSetMaxHealthEvent.target = &target;
    preSetMaxHealthEvent.value = &value;
    _events.RaiseEvent(preSetMaxHealthEvent);

    if (value != _state.players[target].maxHealth)
    {
        _state.players[target].maxHealth = value;
    }
}

void Core::AddCardToHand(std::unique_ptr<cards::Card> card, int playerIndex)
{
    card->OnEnterHand(this, playerIndex);
    _state.players[playerIndex].hand.push_back(std::move(card));
}

std::unique_ptr<cards::Card> Core::RemoveCardFromHand(cards::Card* card, int playerIndex)
{
    for (int i = 0; i < _state.players[playerIndex].hand.size(); i++)
    {
        if (_state.players[playerIndex].hand[i].get() == card)
        {
            std::unique_ptr<cards::Card> movedCard = std::move(_state.players[playerIndex].hand[i]);
            _state.players[playerIndex].hand.erase(_state.players[playerIndex].hand.begin() + i);
            return movedCard;
        }
    }
    return nullptr;
}

void Core::AddCardToActiveCards(std::unique_ptr<cards::Card> card, int playerIndex)
{
    card->OnEnterActiveCards(this, playerIndex);
    _state.players[playerIndex].activeCards.push_back(std::move(card));
}

std::unique_ptr<cards::Card> Core::RemoveCardFromActiveCards(cards::Card* card, int playerIndex)
{
    for (int i = 0; i < _state.players[playerIndex].activeCards.size(); i++)
    {
        if (_state.players[playerIndex].activeCards[i].get() == card)
        {
            std::unique_ptr<cards::Card> movedCard = std::move(_state.players[playerIndex].activeCards[i]);
            _state.players[playerIndex].activeCards.erase(_state.players[playerIndex].activeCards.begin() + i);
            return movedCard;
        }
    }
    return nullptr;
}

bool Core::AddCardToDeck(std::unique_ptr<cards::Card> card)
{
    std::vector<std::unique_ptr<cards::Card>>& deckRef = _state.offenseDeck;
    switch (card->GetCardType())
    {
    case cards::CardType::OFFENSE:
        deckRef = _state.offenseDeck;
        break;
    case cards::CardType::DEFENSE:
        deckRef = _state.defenseDeck;
        break;
    case cards::CardType::UTILITY:
        deckRef = _state.utilityDeck;
        break;
    case cards::CardType::COMBO:
        deckRef = _state.comboDeck;
        break;
    default:
        return false;
    }

    card->OnEnterDeck(this);
    deckRef.push_back(std::move(card));
    return true;
}

std::unique_ptr<cards::Card> Core::RemoveCardFromDeck(cards::Card* card)
{
    std::vector<std::unique_ptr<cards::Card>>& deckRef = _state.offenseDeck;
    switch (card->GetCardType())
    {
    case cards::CardType::OFFENSE:
        deckRef = _state.offenseDeck;
        break;
    case cards::CardType::DEFENSE:
        deckRef = _state.defenseDeck;
        break;
    case cards::CardType::UTILITY:
        deckRef = _state.utilityDeck;
        break;
    case cards::CardType::COMBO:
        deckRef = _state.comboDeck;
        break;
    default:
        return nullptr;
    }

    for (int i = 0; i < deckRef.size(); i++)
    {
        if (deckRef[i].get() == card)
        {
            std::unique_ptr<cards::Card> movedCard = std::move(deckRef[i]);
            deckRef.erase(deckRef.begin() + i);
            return movedCard;
        }
    }
    return nullptr;
}

void Core::AddCardToGraveyard(std::unique_ptr<cards::Card> card)
{
    card->OnEnterGraveyard(this);
    _state.graveyard.push_back(std::move(card));
}

std::unique_ptr<cards::Card> Core::RemoveCardFromGraveyard(cards::Card* card)
{
    for (int i = 0; i < _state.graveyard.size(); i++)
    {
        if (_state.graveyard[i].get() == card)
        {
            std::unique_ptr<cards::Card> movedCard = std::move(_state.graveyard[i]);
            _state.graveyard.erase(_state.graveyard.begin() + i);
            return movedCard;
        }
    }
    return nullptr;
}