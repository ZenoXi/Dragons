#include "Core.h"

#include "GameConstants.h"
#include "Events/TurnEvents.h"
#include "Events/ActionEvents.h"
#include "Events/StatsEvents.h"

#include "Cards/Offense/BloodDonation.h"
#include "Cards/Offense/DeathPoison.h"
#include "Cards/Offense/DesperateStrike.h"
#include "Cards/Offense/FairFight.h"
#include "Cards/Offense/FireMoon.h"
#include "Cards/Offense/HeavySlash.h"
#include "Cards/Offense/HellfireSword.h"
#include "Cards/Offense/LastBreath.h"
#include "Cards/Offense/RapidAttack.h"
#include "Cards/Offense/SacrificialAttack.h"
#include "Cards/Offense/Stab.h"
#include "Cards/Offense/StarCurse.h"
#include "Cards/Offense/VitalSpot.h"
#include "Cards/Offense/WeaponScroll.h"

#include "Cards/Defense/ArmorUp.h"
#include "Cards/Defense/Barrier.h"
#include "Cards/Defense/Cure.h"
#include "Cards/Defense/DivineProtection.h"
#include "Cards/Defense/Healer.h"
#include "Cards/Defense/HelpingHand.h"
#include "Cards/Defense/LifeFlower.h"
#include "Cards/Defense/PeacePact.h"
#include "Cards/Defense/Preparations.h"
#include "Cards/Defense/Recreation.h"
#include "Cards/Defense/ShieldingNet.h"
#include "Cards/Defense/SoothingSpell.h"
#include "Cards/Defense/SpiritShelter.h"
#include "Cards/Defense/TreeOfLife.h"
#include "Cards/Defense/WarBlessing.h"

#include "Cards/Utility/AbsoluteZero.h"
#include "Cards/Utility/Cheating.h"
#include "Cards/Utility/CorpsePuppet.h"
#include "Cards/Utility/DragonEquation.h"
#include "Cards/Utility/DragonFlame.h"
#include "Cards/Utility/DragonPower.h"
#include "Cards/Utility/DragonSight.h"
#include "Cards/Utility/FrostFire.h"
#include "Cards/Utility/HiddenTreasures.h"
#include "Cards/Utility/LifeExchange.h"
#include "Cards/Utility/MindSteal.h"
#include "Cards/Utility/SetUp.h"
#include "Cards/Utility/SummonDead.h"
#include "Cards/Utility/TheThief.h"
#include "Cards/Utility/WeaponOfChoice.h"

#include <algorithm>

Core::Core()
{
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::BloodDonation()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::DeathPoison()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::DesperateStrike()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::FairFight()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::FireMoon()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::HeavySlash()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::HellfireSword()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::LastBreath()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::RapidAttack()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::SacrificialAttack()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::Stab()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::StarCurse()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::VitalSpot()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::WeaponScroll()));

    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::ArmorUp()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::Barrier()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::Cure()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::DivineProtection()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::Healer()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::HelpingHand()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::LifeFlower()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::PeacePact()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::Preparations()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::Recreation()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::ShieldingNet()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::SoothingSpell()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::SpiritShelter()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::TreeOfLife()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::WarBlessing()));

    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::AbsoluteZero()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::Cheating()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::CorpsePuppet()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::DragonEquation()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::DragonFlame()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::DragonPower()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::DragonSight()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::FrostFire()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::HiddenTreasures()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::LifeExchange()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::MindSteal()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::SetUp()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::SummonDead()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::TheThief()));
    _registeredCards.push_back(std::unique_ptr<cards::Card>(new cards::WeaponOfChoice()));

    std::random_device dev;
    _rng = std::mt19937(dev());
}

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
    _state.players[0].handRevealed = false;
    _state.players[0].index = 0;

    _state.players.push_back(Player{});
    _state.players[1].health = GAME_STARTING_HEALTH;
    _state.players[1].maxHealth = GAME_STARTING_MAX_HEALTH;
    _state.players[1].armor = GAME_STARTING_ARMOR;
    _state.players[1].actionsLeft = 0;
    _state.players[1].handRevealed = false;
    _state.players[1].index = 1;

    _state.currentPlayer = 0;
    _state.opposingPlayer = 1;
}

void Core::ClearState()
{

}

void Core::EndTurn()
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

    return canPlay;
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
        return cards::PlayResult::NotPlayed();

    // Emit pre play event
    PreCardPlayedEvent preCardPlayedEvent;
    preCardPlayedEvent.card = card;
    preCardPlayedEvent.actionProps = &actionPropsLocal;
    preCardPlayedEvent.playProps = playProps;
    _events.RaiseEvent(preCardPlayedEvent);

    // Remove card from hand and move it to currently in play card set
    auto cardPtr = RemoveCardFromHand(card, actionPropsLocal.player);
    AddCardToInPlayCards(std::move(cardPtr));

    return _HandlePlayResult(card->Play(this, actionPropsLocal, playProps), card, actionPropsLocal, playProps);
}

cards::PlayResult Core::ResumePlay(UserInputResponse&& response)
{
    return _HandlePlayResult(currentlyPlayingCard->Resume(std::move(response), this, currentActionProperties, currentPlayProperties), currentlyPlayingCard, currentActionProperties, currentPlayProperties);
}

cards::PlayResult Core::_HandlePlayResult(cards::PlayResult result, cards::Card* playedCard, ActionProperties actionProps, cards::PlayProperties* playProps)
{
    if (result.waitForInput)
    {
        currentlyPlayingCard = playedCard;
        currentActionProperties = actionProps;
        currentPlayProperties = playProps;
        return result;
    }
    if (result.discard)
    {
        // Move to graveyard
        auto cardPtr = RemoveCardFromInPlayCards(playedCard);
        AddCardToGraveyard(std::move(cardPtr));
    }
    else if (result.addToActives)
    {
        // Add to active cards
        auto cardPtr = RemoveCardFromInPlayCards(playedCard);
        AddCardToActiveCards(std::move(cardPtr), actionProps.player);
    }
    else
    {
        // Return to hand
        auto cardPtr = RemoveCardFromInPlayCards(playedCard);
        AddCardToHand(std::move(cardPtr), actionProps.player);
    }
    if (!result.notPlayed)
    {
        // Emit post play event
        PostCardPlayedEvent postCardPlayedEvent;
        postCardPlayedEvent.card = playedCard;
        postCardPlayedEvent.actionProps = &actionProps;
        postCardPlayedEvent.playProps = playProps;
        _events.RaiseEvent(postCardPlayedEvent);
    }

    return result;
}

bool Core::CanDrawCard(cards::CardType deck, int playerIndex)
{
    bool canDraw = true;
    CanDrawEvent canDrawEvent;
    canDrawEvent.playerIndex = playerIndex;
    canDrawEvent.deck = deck;
    canDrawEvent.canDraw = &canDraw;
    return canDraw;
}

cards::Card* Core::DrawCard(cards::CardType type, int playerIndex)
{
    return nullptr;
}

cards::Card* Core::DiscardCard(cards::Card* card, int playerIndex)
{
    return nullptr;
}

bool Core::CanPlayComboCard(ComboProperties comboProps)
{
    return CanPlayComboCard(comboProps, std::nullopt, nullptr);
}

bool Core::CanPlayComboCard(ComboProperties comboProps, std::optional<ActionProperties> actionProps, cards::PlayProperties* playProps)
{
    ActionProperties actionPropsLocal;
    if (actionProps)
        actionPropsLocal = actionProps.value();

    // Populate default player/opponent values
    if (actionPropsLocal.player == DEFAULT_PLAYER)
        actionPropsLocal.player = _state.currentPlayer;
    if (actionPropsLocal.opponent == DEFAULT_OPPONENT)
        actionPropsLocal.opponent = _state.opposingPlayer;

    bool canPlay = true;
    for (auto& requiredCardId : comboProps.requiredCards)
    {
        bool cardFound = false;
        for (auto& card : _state.players[comboProps.player].hand)
        {
            if (card->GetCardId() == requiredCardId)
            {
                cardFound = true;
                break;
            }
        }
        if (!cardFound)
        {
            canPlay = false;
            break;
        }
    }

    CanPlayComboEvent canPlayComboEvent;
    canPlayComboEvent.comboProps = comboProps;
    canPlayComboEvent.actionProps = &actionPropsLocal;
    canPlayComboEvent.playProps = playProps;
    canPlayComboEvent.canPlay = &canPlay;
    _events.RaiseEvent(canPlayComboEvent);

    return canPlay;
}

std::vector<cards::Card*> Core::GetCardsForCombo(ComboProperties comboProps)
{
    std::vector<cards::Card*> requiredCardPtrs;
    bool processed = false;
    PreGetComboCardsEvent preGetComboCardsEvent;
    preGetComboCardsEvent.comboProps = comboProps;
    preGetComboCardsEvent.retrievedCards = &requiredCardPtrs;
    preGetComboCardsEvent.processed = &processed;
    _events.RaiseEvent(preGetComboCardsEvent);

    if (!processed)
    {
        for (auto& requiredCardId : comboProps.requiredCards)
        {
            auto& hand = _state.players[comboProps.player].hand;
            for (int i = 0; i < hand.size(); i++)
            {
                if (hand[i]->GetCardId() == requiredCardId)
                {
                    auto cardPtr = RemoveCardFromHand(i, comboProps.player);
                    requiredCardPtrs.push_back(cardPtr.get());
                    AddCardToInPlayCards(std::move(cardPtr));
                    break;
                }
            }
        }
    }

    PostGetComboCardsEvent postGetComboCardsEvent;
    postGetComboCardsEvent.comboProps = comboProps;
    postGetComboCardsEvent.retrievedCards = &requiredCardPtrs;
    _events.RaiseEvent(postGetComboCardsEvent);

    return requiredCardPtrs;
}


DamageResult Core::Damage(DamageProperties props)
{
    PreDamageEvent_BuffPass preDamageEventBuff;
    preDamageEventBuff.props = &props;
    _events.RaiseEvent(preDamageEventBuff);

    PreDamageEvent_NerfPass preDamageEventNerf;
    preDamageEventNerf.props = &props;
    _events.RaiseEvent(preDamageEventNerf);

    PostDamageEvent postDamageEvent;
    postDamageEvent.props = props;

    DamageResult result;

    Player& targetPlayer = _state.players[props.target];
    if (!props.ignoreArmor)
    {
        if (targetPlayer.armor > 0)
        {
            int newArmorValue;
            if (props.amount >= targetPlayer.armor)
            {
                props.amount -= targetPlayer.armor;
                result.removedArmorAmount += targetPlayer.armor;
                newArmorValue = 0;
            }
            else
            {
                newArmorValue = targetPlayer.armor - props.amount;
                result.removedArmorAmount += props.amount;
                props.amount = 0;
            }

            PreArmorChangeEvent preArmorChange;
            preArmorChange.target = props.target;
            preArmorChange.oldValue = targetPlayer.armor;
            preArmorChange.newValue = &newArmorValue;
            preArmorChange.sourceCard = props.sourceCard;
            _events.RaiseEvent(preArmorChange);

            targetPlayer.armor = newArmorValue;

            PostArmorChangeEvent postArmorChange;
            postArmorChange.target = props.target;
            postArmorChange.oldValue = preArmorChange.oldValue;
            postArmorChange.newValue = newArmorValue;
            postArmorChange.sourceCard = props.sourceCard;
            _events.RaiseEvent(postArmorChange);
        }
    }
    if (props.amount > 0)
    {
        int newHealthValue;
        if (props.amount >= targetPlayer.health)
        {
            props.amount -= targetPlayer.health;
            result.removedHealthAmount += targetPlayer.health;
            newHealthValue = 0;
        }
        else
        {
            newHealthValue = targetPlayer.health - props.amount;
            result.removedHealthAmount += props.amount;
            props.amount = 0;
        }

        PreHealthChangeEvent preHealthChange;
        preHealthChange.target = props.target;
        preHealthChange.oldValue = targetPlayer.health;
        preHealthChange.newValue = &newHealthValue;
        preHealthChange.sourceCard = props.sourceCard;
        _events.RaiseEvent(preHealthChange);

        targetPlayer.health = newHealthValue;

        PostHealthChangeEvent postHealthChange;
        postHealthChange.target = props.target;
        postHealthChange.oldValue = preHealthChange.oldValue;
        postHealthChange.newValue = newHealthValue;
        postHealthChange.sourceCard = props.sourceCard;
        _events.RaiseEvent(postHealthChange);
    }

    _events.RaiseEvent(postDamageEvent);
    return result;
}

void Core::Heal(HealProperties props)
{
    PreHealEvent preHealEvent;
    preHealEvent.props = &props;
    _events.RaiseEvent(preHealEvent);

    if (props.amount <= 0)
        return;

    int newHealthValue = _state.players[props.target].health + props.amount;
    if (newHealthValue > _state.players[props.target].maxHealth)
        newHealthValue = _state.players[props.target].maxHealth;

    PreHealthChangeEvent preHealthChange;
    preHealthChange.target = props.target;
    preHealthChange.oldValue = _state.players[props.target].health;
    preHealthChange.newValue = &newHealthValue;
    preHealthChange.sourceCard = props.sourceCard;
    _events.RaiseEvent(preHealthChange);

    _state.players[props.target].health = newHealthValue;

    PostHealthChangeEvent postHealthChange;
    postHealthChange.target = props.target;
    postHealthChange.oldValue = preHealthChange.oldValue;
    postHealthChange.newValue = newHealthValue;
    postHealthChange.sourceCard = props.sourceCard;
    _events.RaiseEvent(postHealthChange);

    PostHealEvent postHealEvent;
    postHealEvent.props = props;
    _events.RaiseEvent(postHealEvent);
}

void Core::AddArmor(AddArmorProperties props)
{
    PreAddArmorEvent preAddArmorEvent;
    preAddArmorEvent.props = &props;
    _events.RaiseEvent(preAddArmorEvent);

    if (props.amount <= 0)
        return;

    int newArmorValue = _state.players[props.target].armor + props.amount;

    PreArmorChangeEvent preArmorChange;
    preArmorChange.target = props.target;
    preArmorChange.oldValue = _state.players[props.target].armor;
    preArmorChange.newValue = &newArmorValue;
    preArmorChange.sourceCard = props.sourceCard;
    _events.RaiseEvent(preArmorChange);

    _state.players[props.target].armor = newArmorValue;

    PostArmorChangeEvent postArmorChange;
    postArmorChange.target = props.target;
    postArmorChange.oldValue = preArmorChange.oldValue;
    postArmorChange.newValue = newArmorValue;
    postArmorChange.sourceCard = props.sourceCard;
    _events.RaiseEvent(postArmorChange);

    PostAddArmorEvent postAddArmorEvent;
    postAddArmorEvent.props = props;
    _events.RaiseEvent(postAddArmorEvent);
}

void Core::DestroyArmor(int target)
{
    bool cancel = false;
    PreDestroyArmorEvent preDestroyArmorEvent;
    preDestroyArmorEvent.target = &target;
    preDestroyArmorEvent.cancel = &cancel;
    _events.RaiseEvent(preDestroyArmorEvent);

    if (cancel)
        return;

    int newArmorValue = 0;

    PreArmorChangeEvent preArmorChange;
    preArmorChange.target = target;
    preArmorChange.oldValue = _state.players[target].armor;
    preArmorChange.newValue = &newArmorValue;
    preArmorChange.sourceCard = nullptr;
    _events.RaiseEvent(preArmorChange);

    _state.players[target].armor = newArmorValue;

    PostArmorChangeEvent postArmorChange;
    postArmorChange.target = target;
    postArmorChange.oldValue = preArmorChange.oldValue;
    postArmorChange.newValue = newArmorValue;
    postArmorChange.sourceCard = nullptr;
    _events.RaiseEvent(postArmorChange);
}

void Core::SetMaxHealth(int target, int value, bool force)
{
    int newMaxHealthValue = value;

    PreMaxHealthChangeEvent preMaxHealthChange;
    preMaxHealthChange.target = target;
    preMaxHealthChange.oldValue = _state.players[target].maxHealth;
    preMaxHealthChange.newValue = &newMaxHealthValue;
    _events.RaiseEvent(preMaxHealthChange);

    if (force)
        newMaxHealthValue = value;
    _state.players[target].maxHealth = newMaxHealthValue;

    PostMaxHealthChangeEvent postMaxHealthChange;
    postMaxHealthChange.target = target;
    postMaxHealthChange.oldValue = preMaxHealthChange.oldValue;
    postMaxHealthChange.newValue = newMaxHealthValue;
    _events.RaiseEvent(postMaxHealthChange);

    if (_state.players[target].health > _state.players[target].maxHealth)
        SetHealth(target, _state.players[target].maxHealth, true);
}

void Core::SetHealth(int target, int value, bool force)
{
    int newHealthValue = value;

    PreHealthChangeEvent preHealthChange;
    preHealthChange.target = target;
    preHealthChange.oldValue = _state.players[target].health;
    preHealthChange.newValue = &newHealthValue;
    _events.RaiseEvent(preHealthChange);

    if (force)
        newHealthValue = value;
    _state.players[target].health = newHealthValue;

    PostHealthChangeEvent postHealthChange;
    postHealthChange.target = target;
    postHealthChange.oldValue = preHealthChange.oldValue;
    postHealthChange.newValue = newHealthValue;
    _events.RaiseEvent(postHealthChange);
}

void Core::SetArmor(int target, int value, bool force)
{
    int newArmorValue = value;

    PreArmorChangeEvent preArmorChange;
    preArmorChange.target = target;
    preArmorChange.oldValue = _state.players[target].armor;
    preArmorChange.newValue = &newArmorValue;
    _events.RaiseEvent(preArmorChange);

    if (force)
        newArmorValue = value;
    _state.players[target].armor = newArmorValue;

    PostArmorChangeEvent postArmorChange;
    postArmorChange.target = target;
    postArmorChange.oldValue = preArmorChange.oldValue;
    postArmorChange.newValue = newArmorValue;
    _events.RaiseEvent(postArmorChange);
}

void Core::SetActionCount(int target, int amount)
{
    _state.players[target].actionsLeft = amount;
}

void Core::AddActions(int target, int amount)
{
    _state.players[target].actionsLeft += amount;
}

void Core::AddExtraAction(int target, ExtraAction action)
{
    _state.players[target].extraActions.push_back(action);
}

void Core::AddCardToHand(std::unique_ptr<cards::Card> card, int playerIndex)
{
    if (card->IsVirtual() && !card->IsAllowedSet(cards::CardSet{ cards::CardSets::HAND, playerIndex }))
    {
        AddCardToDestroyedCards(std::move(card));
        return;
    }

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

std::unique_ptr<cards::Card> Core::RemoveCardFromHand(int cardIndex, int playerIndex)
{
    std::unique_ptr<cards::Card> movedCard = std::move(_state.players[playerIndex].hand[cardIndex]);
    _state.players[playerIndex].hand.erase(_state.players[playerIndex].hand.begin() + cardIndex);
    return movedCard;
}

void Core::AddCardToActiveCards(std::unique_ptr<cards::Card> card, int playerIndex)
{
    if (card->IsVirtual() && !card->IsAllowedSet(cards::CardSet{ cards::CardSets::ACTIVE_CARDS, playerIndex }))
    {
        AddCardToDestroyedCards(std::move(card));
        return;
    }

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

std::unique_ptr<cards::Card> Core::RemoveCardFromActiveCards(int cardIndex, int playerIndex)
{
    std::unique_ptr<cards::Card> movedCard = std::move(_state.players[playerIndex].activeCards[cardIndex]);
    _state.players[playerIndex].activeCards.erase(_state.players[playerIndex].activeCards.begin() + cardIndex);
    return movedCard;
}

bool Core::AddCardToDeck(std::unique_ptr<cards::Card> card)
{
    if (card->IsVirtual() && !card->IsAllowedSet(cards::CardSet{ cards::CardSets::DECK }))
    {
        AddCardToDestroyedCards(std::move(card));
        return false;
    }

    auto& deckRef = _ResolveDeckFromType(card->GetCardType());
    card->OnEnterDeck(this);
    deckRef.push_back(std::move(card));
    return true;
}

std::unique_ptr<cards::Card> Core::RemoveCardFromDeck(cards::Card* card)
{
    auto& deckRef = _ResolveDeckFromType(card->GetCardType());
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

std::unique_ptr<cards::Card> Core::RemoveCardFromDeck(cards::CardType deck, int cardIndex)
{
    auto& deckRef = _ResolveDeckFromType(deck);
    std::unique_ptr<cards::Card> movedCard = std::move(deckRef[cardIndex]);
    deckRef.erase(deckRef.begin() + cardIndex);
    return movedCard;
}

void Core::AddCardToGraveyard(std::unique_ptr<cards::Card> card)
{
    if (card->IsVirtual() && !card->IsAllowedSet(cards::CardSet{ cards::CardSets::GRAVEYARD }))
    {
        AddCardToDestroyedCards(std::move(card));
        return;
    }

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

std::unique_ptr<cards::Card> Core::RemoveCardFromGraveyard(int cardIndex)
{
    std::unique_ptr<cards::Card> movedCard = std::move(_state.graveyard[cardIndex]);
    _state.graveyard.erase(_state.graveyard.begin() + cardIndex);
    return movedCard;
}

void Core::AddCardToInPlayCards(std::unique_ptr<cards::Card> card)
{
    if (card->IsVirtual() && !card->IsAllowedSet(cards::CardSet{ cards::CardSets::IN_PLAY }))
    {
        AddCardToDestroyedCards(std::move(card));
        return;
    }

    card->OnEnterGraveyard(this);
    _state.inPlayCards.push_back(std::move(card));
}

std::unique_ptr<cards::Card> Core::RemoveCardFromInPlayCards(cards::Card* card)
{
    for (int i = 0; i < _state.inPlayCards.size(); i++)
    {
        if (_state.inPlayCards[i].get() == card)
        {
            std::unique_ptr<cards::Card> movedCard = std::move(_state.inPlayCards[i]);
            _state.inPlayCards.erase(_state.inPlayCards.begin() + i);
            return movedCard;
        }
    }
    return nullptr;
}

std::unique_ptr<cards::Card> Core::RemoveCardFromInPlayCards(int cardIndex)
{
    std::unique_ptr<cards::Card> movedCard = std::move(_state.inPlayCards[cardIndex]);
    _state.inPlayCards.erase(_state.inPlayCards.begin() + cardIndex);
    return movedCard;
}

void Core::AddCardToDestroyedCards(std::unique_ptr<cards::Card> card)
{
    card->OnEnterGraveyard(this);
    _state.destroyedCards.push_back(std::move(card));
}

std::unique_ptr<cards::Card> Core::RemoveCardFromDestroyedCards(cards::Card* card)
{
    for (int i = 0; i < _state.destroyedCards.size(); i++)
    {
        if (_state.destroyedCards[i].get() == card)
        {
            std::unique_ptr<cards::Card> movedCard = std::move(_state.destroyedCards[i]);
            _state.destroyedCards.erase(_state.destroyedCards.begin() + i);
            return movedCard;
        }
    }
    return nullptr;
}

std::unique_ptr<cards::Card> Core::RemoveCardFromDestroyedCards(int cardIndex)
{
    std::unique_ptr<cards::Card> movedCard = std::move(_state.destroyedCards[cardIndex]);
    _state.destroyedCards.erase(_state.destroyedCards.begin() + cardIndex);
    return movedCard;
}

void Core::AddCardToDisplayedCards(DisplayInfo displayInfo)
{
    _state.displayedCards.push_back(displayInfo);
}

bool Core::RemoveCardFromDisplayedCards(cards::Card* card)
{
    for (int i = 0; i < _state.displayedCards.size(); i++)
    {
        if (_state.displayedCards[i].card == card)
        {
            _state.displayedCards.erase(_state.displayedCards.begin() + i);
            return true;
        }
    }
    return false;
}

void Core::RemoveCardFromDisplayedCards(int cardIndex)
{
    _state.displayedCards.erase(_state.displayedCards.begin() + cardIndex);
}

bool Core::ModifyDisplayedCard(DisplayInfo newDisplayInfo)
{
    for (int i = 0; i < _state.displayedCards.size(); i++)
    {
        if (_state.displayedCards[i].card == newDisplayInfo.card)
        {
            _state.displayedCards[i] = newDisplayInfo;
            return true;
        }
    }
    return false;
}

void Core::ClearDisplayedCards()
{
    while (!_state.displayedCards.empty())
    {
        _state.displayedCards.erase(_state.displayedCards.begin());
        // Add event invoke on each erase
    }
}

cards::CardSet Core::GetCardSet(cards::Card* cardToFind)
{
    for (int i = 0; i < _state.players.size(); i++)
    {
        // Player hand
        for (auto& card : _state.players[i].hand)
        {
            if (card.get() == cardToFind)
            {
                return cards::CardSet{ cards::CardSets::HAND, i };
            }
        }

        // Player active cards
        for (auto& card : _state.players[i].activeCards)
        {
            if (card.get() == cardToFind)
            {
                return cards::CardSet{ cards::CardSets::ACTIVE_CARDS, i };
            }
        }

    }

    // Deck
    auto& deckRef = _ResolveDeckFromType(cardToFind->GetCardType());
    for (auto& card : deckRef)
    {
        if (card.get() == cardToFind)
        {
            return cards::CardSet{ cards::CardSets::DECK, -1 };
        }
    }

    // Graveyard
    for (auto& card : _state.graveyard)
    {
        if (card.get() == cardToFind)
        {
            return cards::CardSet{ cards::CardSets::GRAVEYARD, -1 };
        }
    }

    // Destroyed
    for (auto& card : _state.destroyedCards)
    {
        if (card.get() == cardToFind)
        {
            return cards::CardSet{ cards::CardSets::DESTROYED, -1 };
        }
    }

    return cards::CardSet{ cards::CardSets::NONE, -1 };
}

void Core::ShuffleDeck(cards::CardType type)
{
    auto& deckRef = _ResolveDeckFromType(type);
    std::shuffle(deckRef.begin(), deckRef.end(), _rng);
}

int Core::GenerateRandomNumber(int fromInclusive, int toExclusive)
{
    auto dist = std::uniform_int_distribution<int>(fromInclusive, toExclusive - 1);
    return dist(_rng);
}

void Core::RevealHand(int target, std::string revealSource)
{
    _state.players[target].handRevealed = true;
    _state.players[target].revealSources.push_back(revealSource);
}

void Core::HideHand(int target, std::string revealSource)
{
    for (int i = 0; i < _state.players[target].revealSources.size(); i++)
    {
        if (_state.players[target].revealSources[i] == revealSource)
        {
            _state.players[target].revealSources.erase(_state.players[target].revealSources.begin() + i);
            break;
        }
    }

    if (_state.players[target].revealSources.empty())
        _state.players[target].handRevealed = false;
}

std::unique_ptr<cards::Card> Core::CreateCard(CardId cardId)
{
    for (auto& card : _registeredCards)
    {
        if (card->GetCardId() == cardId)
        {
            return card->CreateInstance();
        }
    }
    return nullptr;
}

std::vector<std::unique_ptr<cards::Card>>& Core::_ResolveDeckFromType(cards::CardType type)
{
    switch (type)
    {
    case cards::CardType::OFFENSE:
        return _state.offenseDeck;
    case cards::CardType::DEFENSE:
        return _state.defenseDeck;
    case cards::CardType::UTILITY:
        return _state.utilityDeck;
    case cards::CardType::COMBO:
        return _state.comboDeck;
    }
}