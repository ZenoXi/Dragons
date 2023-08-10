#include "Core.h"

#include "GameConstants.h"
#include "Events/TurnEvents.h"
#include "Events/ActionEvents.h"
#include "Events/StatsEvents.h"

#include "Cards/Offense/BloodDonation.h"
#include "Cards/Offense/DeathPoison.h"
#include "Cards/Offense/DesperateStrike.h"
#include "Cards/Offense/DevilsDeal.h"
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

#include "Cards/Combo/Apocalypse.h"
#include "Cards/Combo/AstroInfluence.h"
#include "Cards/Combo/CounterScale.h"
#include "Cards/Combo/DeathFee.h"
#include "Cards/Combo/DifferentSides.h"
#include "Cards/Combo/DoubleStrike.h"
#include "Cards/Combo/DragonSword.h"
#include "Cards/Combo/DragonWeapons.h"
#include "Cards/Combo/DreamLand.h"
#include "Cards/Combo/ElementalDragon.h"
#include "Cards/Combo/FakeOut.h"
#include "Cards/Combo/FinalHour.h"
#include "Cards/Combo/InfernoLifeline.h"
#include "Cards/Combo/MagicTrick.h"
#include "Cards/Combo/NatureSpell.h"
#include "Cards/Combo/Omniclaustra.h"
#include "Cards/Combo/PowerUp.h"
#include "Cards/Combo/ReEquip.h"
#include "Cards/Combo/Ritual.h"
#include "Cards/Combo/Shackling.h"
#include "Cards/Combo/ShadowAssassin.h"
#include "Cards/Combo/Soulstealer.h"
#include "Cards/Combo/Superiority.h"
#include "Cards/Combo/SwapPact.h"
#include "Cards/Combo/ThreeGodsStrike.h"
#include "Cards/Combo/TotalEntanglement.h"
#include "Cards/Combo/UltimateDefense.h"
#include "Cards/Combo/UltimateExchange.h"
#include "Cards/Combo/Underworld.h"
#include "Cards/Combo/Vampirism.h"

#include <algorithm>

Core::Core()
{
    _registeredCards.push_back(std::make_unique<cards::BloodDonation>());
    _registeredCards.push_back(std::make_unique<cards::DeathPoison>());
    _registeredCards.push_back(std::make_unique<cards::DesperateStrike>());
    _registeredCards.push_back(std::make_unique<cards::DevilsDeal>());
    _registeredCards.push_back(std::make_unique<cards::FairFight>());
    _registeredCards.push_back(std::make_unique<cards::FireMoon>());
    _registeredCards.push_back(std::make_unique<cards::HeavySlash>());
    _registeredCards.push_back(std::make_unique<cards::HellfireSword>());
    _registeredCards.push_back(std::make_unique<cards::LastBreath>());
    _registeredCards.push_back(std::make_unique<cards::RapidAttack>());
    _registeredCards.push_back(std::make_unique<cards::SacrificialAttack>());
    _registeredCards.push_back(std::make_unique<cards::Stab>());
    _registeredCards.push_back(std::make_unique<cards::StarCurse>());
    _registeredCards.push_back(std::make_unique<cards::VitalSpot>());
    _registeredCards.push_back(std::make_unique<cards::WeaponScroll>());

    _registeredCards.push_back(std::make_unique<cards::ArmorUp>());
    _registeredCards.push_back(std::make_unique<cards::Barrier>());
    _registeredCards.push_back(std::make_unique<cards::Cure>());
    _registeredCards.push_back(std::make_unique<cards::DivineProtection>());
    _registeredCards.push_back(std::make_unique<cards::Healer>());
    _registeredCards.push_back(std::make_unique<cards::HelpingHand>());
    _registeredCards.push_back(std::make_unique<cards::LifeFlower>());
    _registeredCards.push_back(std::make_unique<cards::PeacePact>());
    _registeredCards.push_back(std::make_unique<cards::Preparations>());
    _registeredCards.push_back(std::make_unique<cards::Recreation>());
    _registeredCards.push_back(std::make_unique<cards::ShieldingNet>());
    _registeredCards.push_back(std::make_unique<cards::SoothingSpell>());
    _registeredCards.push_back(std::make_unique<cards::SpiritShelter>());
    _registeredCards.push_back(std::make_unique<cards::TreeOfLife>());
    _registeredCards.push_back(std::make_unique<cards::WarBlessing>());

    _registeredCards.push_back(std::make_unique<cards::AbsoluteZero>());
    _registeredCards.push_back(std::make_unique<cards::Cheating>());
    _registeredCards.push_back(std::make_unique<cards::CorpsePuppet>());
    _registeredCards.push_back(std::make_unique<cards::DragonEquation>());
    _registeredCards.push_back(std::make_unique<cards::DragonFlame>());
    _registeredCards.push_back(std::make_unique<cards::DragonPower>());
    _registeredCards.push_back(std::make_unique<cards::DragonSight>());
    _registeredCards.push_back(std::make_unique<cards::FrostFire>());
    _registeredCards.push_back(std::make_unique<cards::HiddenTreasures>());
    _registeredCards.push_back(std::make_unique<cards::LifeExchange>());
    _registeredCards.push_back(std::make_unique<cards::MindSteal>());
    _registeredCards.push_back(std::make_unique<cards::SetUp>());
    _registeredCards.push_back(std::make_unique<cards::SummonDead>());
    _registeredCards.push_back(std::make_unique<cards::TheThief>());
    _registeredCards.push_back(std::make_unique<cards::WeaponOfChoice>());

    _registeredCards.push_back(std::make_unique<cards::Apocalypse>());
    _registeredCards.push_back(std::make_unique<cards::AstroInfluence>());
    _registeredCards.push_back(std::make_unique<cards::CounterScale>());
    _registeredCards.push_back(std::make_unique<cards::DeathFee>());
    _registeredCards.push_back(std::make_unique<cards::DifferentSides>());
    _registeredCards.push_back(std::make_unique<cards::DoubleStrike>());
    _registeredCards.push_back(std::make_unique<cards::DragonSword>());
    _registeredCards.push_back(std::make_unique<cards::DragonWeapons>());
    _registeredCards.push_back(std::make_unique<cards::DreamLand>());
    _registeredCards.push_back(std::make_unique<cards::ElementalDragon>());
    _registeredCards.push_back(std::make_unique<cards::FakeOut>());
    _registeredCards.push_back(std::make_unique<cards::FinalHour>());
    _registeredCards.push_back(std::make_unique<cards::InfernoLifeline>());
    _registeredCards.push_back(std::make_unique<cards::MagicTrick>());
    _registeredCards.push_back(std::make_unique<cards::NatureSpell>());
    _registeredCards.push_back(std::make_unique<cards::Omniclaustra>());
    _registeredCards.push_back(std::make_unique<cards::PowerUp>());
    _registeredCards.push_back(std::make_unique<cards::ReEquip>());
    _registeredCards.push_back(std::make_unique<cards::Ritual>());
    _registeredCards.push_back(std::make_unique<cards::Shackling>());
    _registeredCards.push_back(std::make_unique<cards::ShadowAssassin>());
    _registeredCards.push_back(std::make_unique<cards::Soulstealer>());
    _registeredCards.push_back(std::make_unique<cards::Superiority>());
    _registeredCards.push_back(std::make_unique<cards::SwapPact>());
    _registeredCards.push_back(std::make_unique<cards::ThreeGodsStrike>());
    _registeredCards.push_back(std::make_unique<cards::TotalEntanglement>());
    _registeredCards.push_back(std::make_unique<cards::UltimateDefense>());
    _registeredCards.push_back(std::make_unique<cards::UltimateExchange>());
    _registeredCards.push_back(std::make_unique<cards::Underworld>());
    _registeredCards.push_back(std::make_unique<cards::Vampirism>());

    std::random_device dev;
    _rng = std::mt19937(dev());
}

void Core::InitState()
{
    ClearState();

    // Init decks
    for (auto& card : _registeredCards)
    {
        AddCardToDeck(card->CreateInstance());
        if (card->GetCardType() != cards::CardType::COMBO)
            AddCardToDeck(card->CreateInstance());
    }
    ShuffleDeck(cards::CardType::OFFENSE);
    ShuffleDeck(cards::CardType::DEFENSE);
    ShuffleDeck(cards::CardType::UTILITY);
    ShuffleDeck(cards::CardType::COMBO);

    // Init players
    _state.players.push_back(Player{});
    _state.players[0].health = GAME_STARTING_HEALTH;
    _state.players[0].maxHealth = GAME_STARTING_MAX_HEALTH;
    _state.players[0].armor = GAME_STARTING_ARMOR;
    _state.players[0].actionsLeft = 0;
    _state.players[0].handRevealed = false;
    _state.players[0].index = 0;
    AddCardToHand(std::make_unique<cards::Stab>(), 0);
    AddCardToHand(std::make_unique<cards::BloodDonation>(), 0);
    AddCardToHand(std::make_unique<cards::WeaponOfChoice>(), 0);
    AddCardToHand(std::make_unique<cards::DragonSight>(), 0);
    AddCardToHand(std::make_unique<cards::SummonDead>(), 0);
    AddCardToHand(std::make_unique<cards::FrostFire>(), 0);
    //AddCardToHand(std::make_unique<cards::ElementalDragon>(), 0);
    //AddCardToHand(std::make_unique<cards::HeavySlash>(), 0);
    //AddCardToHand(std::make_unique<cards::ShieldingNet>(), 0);
    //AddCardToHand(std::make_unique<cards::CorpsePuppet>(), 0);
    //AddCardToActiveCards(std::make_unique<cards::ElementalDragon>(), 0);
    //AddCardToActiveCards(std::make_unique<cards::HeavySlash>(), 0);
    //AddCardToActiveCards(std::make_unique<cards::ShieldingNet>(), 0);
    //AddCardToActiveCards(std::make_unique<cards::CorpsePuppet>(), 0);

    _state.players.push_back(Player{});
    _state.players[1].health = GAME_STARTING_HEALTH;
    _state.players[1].maxHealth = GAME_STARTING_MAX_HEALTH;
    _state.players[1].armor = GAME_STARTING_ARMOR;
    _state.players[1].actionsLeft = 0;
    _state.players[1].handRevealed = false;
    _state.players[1].index = 1;
    AddCardToHand(std::make_unique<cards::Barrier>(), 1);
    AddCardToHand(std::make_unique<cards::Barrier>(), 1);
    //AddCardToHand(std::make_unique<cards::ElementalDragon>(), 1);
    //AddCardToHand(std::make_unique<cards::HeavySlash>(), 1);
    //AddCardToHand(std::make_unique<cards::ShieldingNet>(), 1);
    //AddCardToHand(std::make_unique<cards::CorpsePuppet>(), 1);
    //AddCardToActiveCards(std::make_unique<cards::ElementalDragon>(), 1);
    //AddCardToActiveCards(std::make_unique<cards::HeavySlash>(), 1);
    //AddCardToActiveCards(std::make_unique<cards::ShieldingNet>(), 1);
    //AddCardToActiveCards(std::make_unique<cards::CorpsePuppet>(), 1);

    AddCardToGraveyard(std::make_unique<cards::Stab>());
    AddCardToGraveyard(std::make_unique<cards::BloodDonation>());
    AddCardToGraveyard(std::make_unique<cards::WeaponOfChoice>());
    AddCardToGraveyard(std::make_unique<cards::Underworld>());
    AddCardToGraveyard(std::make_unique<cards::SummonDead>());
    AddCardToGraveyard(std::make_unique<cards::DragonFlame>());
    AddCardToGraveyard(std::make_unique<cards::DragonSight>());
    AddCardToGraveyard(std::make_unique<cards::SummonDead>());
    AddCardToGraveyard(std::make_unique<cards::DragonFlame>());
    AddCardToGraveyard(std::make_unique<cards::DragonSight>());
    AddCardToGraveyard(std::make_unique<cards::SummonDead>());
    AddCardToGraveyard(std::make_unique<cards::DragonFlame>());
    AddCardToGraveyard(std::make_unique<cards::DragonSight>());

    _state.currentPlayer = 0;
    _state.opposingPlayer = 1;

    _state.players[0].actionsLeft = 2;
}

void Core::ClearState()
{

}

void Core::EndTurn()
{
    // Apply fatigue
    DamageProperties props;
    props.target = _state.currentPlayer;
    props.amount = 1;
    props.fatigue = true;
    Damage(props);


    TurnEndEvent turnEndEvent;
    turnEndEvent.playerIndex = _state.currentPlayer;
    turnEndEvent.opponentIndex = _state.opposingPlayer;
    _events.RaiseEvent(turnEndEvent);

    if (_state.currentPlayer == 0)
    {
        _state.currentPlayer = 1;
        _state.opposingPlayer = 0;
    }
    else
    {
        _state.currentPlayer = 0;
        _state.opposingPlayer = 1;
    }

    _state.players[_state.currentPlayer].actionsLeft = 2;
    _state.players[_state.currentPlayer].extraActions.clear();

    TurnBeginEvent turnBeginEvent;
    turnBeginEvent.playerIndex = _state.currentPlayer;
    turnBeginEvent.opponentIndex = _state.opposingPlayer;
    _events.RaiseEvent(turnBeginEvent);
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

    // Validate action cost
    if (!actionPropsLocal.freeAction)
    {
        int playsAvailable = _state.players[actionPropsLocal.player].actionsLeft;
        for (auto& extraAction : _state.players[actionPropsLocal.player].extraActions)
        {
            if (extraAction.play)
                playsAvailable++;
        }
        if (playsAvailable < card->GetActionCost())
            return false;
    }

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

    Player& player = _state.players[actionPropsLocal.player];

    // Consume action
    // This needs to be done before invoking card play code, because otherwise
    // cards that add an extra play (eg. Soothing Spell) will consume it themselves
    if (!actionPropsLocal.freeAction)
    {
        _actionsConsumed = 0;
        _extraActionsConsumed.clear();
        for (int j = 0; j < card->GetActionCost(); j++)
        {
            bool extraActionUsed = false;
            if (!player.extraActions.empty())
            {
                int exActionIndex = -1;
                int exActionWeight = 99;
                for (int i = 0; i < player.extraActions.size(); i++)
                {
                    if (player.extraActions[i].play && player.extraActions[i].Weight() < exActionWeight)
                    {
                        exActionIndex = i;
                        exActionWeight = player.extraActions[i].Weight();
                    }
                }

                if (exActionIndex != -1)
                {
                    _extraActionsConsumed.push_back(player.extraActions[exActionIndex]);
                    player.extraActions.erase(player.extraActions.begin() + exActionIndex);
                    extraActionUsed = true;
                }
            }
            if (!extraActionUsed)
            {
                _actionsConsumed++;
                player.actionsLeft--;
            }
        }

        _events.RaiseEvent(ActionCountChangedEvent{});
    }

    // Emit pre play event
    PreCardPlayedEvent preCardPlayedEvent;
    preCardPlayedEvent.card = card;
    preCardPlayedEvent.actionProps = &actionPropsLocal;
    preCardPlayedEvent.playProps = &playProps;
    _events.RaiseEvent(preCardPlayedEvent);

    // Remove card from hand and move it to currently in play card set
    auto cardPtr = RemoveCardFromHand(card, actionPropsLocal.player);
    AddCardToInPlayCards(std::move(cardPtr));

    return _HandlePlayResult(card->Play(this, actionPropsLocal, playProps), card, actionPropsLocal, playProps);
}

cards::PlayResult Core::ResumePlay(UserInputResponse&& response)
{
    return _HandlePlayResult(
        _currentlyPlayingCards.back().card->Resume(std::move(response), this, _currentlyPlayingCards.back().actionProps, _currentlyPlayingCards.back().playProps),
        _currentlyPlayingCards.back().card,
        _currentlyPlayingCards.back().actionProps,
        _currentlyPlayingCards.back().playProps
    );
}

void Core::MoveCardAfterPlay(const cards::PlayResult& result, cards::Card* playedCard, ActionProperties actionProps, cards::PlayProperties* playProps)
{
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
}

cards::PlayResult Core::_HandlePlayResult(cards::PlayResult result, cards::Card* playedCard, ActionProperties actionProps, cards::PlayProperties* playProps)
{
    if (result.waitForInput)
    {
        _currentlyPlayingCards.push_back({ playedCard, actionProps, playProps });
        return result;
    }
    else
    {
        if (!_currentlyPlayingCards.empty() && _currentlyPlayingCards.back().card == playedCard)
            _currentlyPlayingCards.pop_back();
    }

    MoveCardAfterPlay(result, playedCard, actionProps, playProps);

    Player& player = _state.players[actionProps.player];
    if (!result.notPlayed)
    {
        // Emit post play event
        PostCardPlayedEvent postCardPlayedEvent;
        postCardPlayedEvent.card = playedCard;
        postCardPlayedEvent.actionProps = &actionProps;
        postCardPlayedEvent.playProps = playProps;
        _events.RaiseEvent(postCardPlayedEvent);

        // End turn
        // Check for end of turn should probably be done by checking only after primary card play has finished
        // because thats the only time it makes sense to do that. Atm !actionProps.freeAction is a workaround
        // because I can't be bothered to do it properly (and dont really have the time)
        if (!actionProps.freeAction && player.actionsLeft == 0 && player.extraActions.empty())
        {
            EndTurn();
        }
    }
    else
    {
        // Return consumed actions
        player.actionsLeft += _actionsConsumed;
        for (auto& action : _extraActionsConsumed)
            player.extraActions.push_back(action);

        _events.RaiseEvent(ActionCountChangedEvent{});
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
    _events.RaiseEvent(canDrawEvent);
    return canDraw;
}

cards::Card* Core::DrawCard(cards::CardType type, int playerIndex, bool consumeAction)
{
    auto& deckRef = _ResolveDeckFromType(type);
    if (deckRef.empty())
        return nullptr;

    auto cardPtr = RemoveCardFromDeck(type, deckRef.size() - 1);
    cards::Card* card = cardPtr.get();
    AddCardToHand(std::move(cardPtr), playerIndex);

    Player& player = _state.players[playerIndex];

    // Consume action
    if (consumeAction)
    {
        bool extraActionUsed = false;
        if (!player.extraActions.empty())
        {
            int exActionIndex = -1;
            int exActionWeight = 99;
            for (int i = 0; i < player.extraActions.size(); i++)
            {
                if (player.extraActions[i].draw && player.extraActions[i].Weight() < exActionWeight)
                {
                    exActionIndex = i;
                    exActionWeight = player.extraActions[i].Weight();
                }
            }

            if (exActionIndex != -1)
            {
                player.extraActions.erase(player.extraActions.begin() + exActionIndex);
                extraActionUsed = true;
            }
        }
        if (!extraActionUsed)
            player.actionsLeft--;

        _events.RaiseEvent(ActionCountChangedEvent{});

        // End turn
        if (player.actionsLeft == 0 && player.extraActions.empty())
        {
            EndTurn();
        }
    }

    return card;
}

cards::Card* Core::DiscardCard(cards::Card* card, int playerIndex, bool consumeAction)
{
    auto cardPtr = RemoveCardFromHand(card, playerIndex);
    if (cardPtr)
    {
        AddCardToGraveyard(std::move(cardPtr));

        Player& player = _state.players[playerIndex];

        // Consume action
        if (consumeAction)
        {
            bool extraActionUsed = false;
            if (!player.extraActions.empty())
            {
                int exActionIndex = -1;
                int exActionWeight = 99;
                for (int i = 0; i < player.extraActions.size(); i++)
                {
                    if (player.extraActions[i].discard && player.extraActions[i].Weight() < exActionWeight)
                    {
                        exActionIndex = i;
                        exActionWeight = player.extraActions[i].Weight();
                    }
                }

                if (exActionIndex != -1)
                {
                    player.extraActions.erase(player.extraActions.begin() + exActionIndex);
                    extraActionUsed = true;
                }
            }
            if (!extraActionUsed)
                player.actionsLeft--;

            _events.RaiseEvent(ActionCountChangedEvent{});

            // End turn
            if (player.actionsLeft == 0 && player.extraActions.empty())
            {
                EndTurn();
            }
        }

        return card;
    }
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
    if (!props.ignoreArmor && !props.fatigue)
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

    postDamageEvent.result = result;
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
    _events.RaiseEvent(ActionCountChangedEvent{});
}

void Core::AddActions(int target, int amount)
{
    _state.players[target].actionsLeft += amount;
    _events.RaiseEvent(ActionCountChangedEvent{});
}

void Core::AddExtraAction(int target, ExtraAction action)
{
    _state.players[target].extraActions.push_back(action);
    _events.RaiseEvent(ActionCountChangedEvent{});
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

    CardEnterHandEvent event;
    event.card = _state.players[playerIndex].hand.back().get();
    event.playerIndex = playerIndex;
    _events.RaiseEvent(event);
}

std::unique_ptr<cards::Card> Core::RemoveCardFromHand(cards::Card* card, int playerIndex)
{
    for (int i = 0; i < _state.players[playerIndex].hand.size(); i++)
    {
        if (_state.players[playerIndex].hand[i].get() == card)
        {
            return RemoveCardFromHand(i, playerIndex);
        }
    }
    return nullptr;
}

std::unique_ptr<cards::Card> Core::RemoveCardFromHand(int cardIndex, int playerIndex)
{
    std::unique_ptr<cards::Card> movedCard = std::move(_state.players[playerIndex].hand[cardIndex]);
    _state.players[playerIndex].hand.erase(_state.players[playerIndex].hand.begin() + cardIndex);

    CardLeaveHandEvent event;
    event.card = movedCard.get();
    event.playerIndex = playerIndex;
    _events.RaiseEvent(event);

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

    CardEnterActivesEvent event;
    event.card = _state.players[playerIndex].activeCards.back().get();
    event.playerIndex = playerIndex;
    _events.RaiseEvent(event);
}

std::unique_ptr<cards::Card> Core::RemoveCardFromActiveCards(cards::Card* card, int playerIndex)
{
    for (int i = 0; i < _state.players[playerIndex].activeCards.size(); i++)
    {
        if (_state.players[playerIndex].activeCards[i].get() == card)
        {
            return RemoveCardFromActiveCards(i, playerIndex);
        }
    }
    return nullptr;
}

std::unique_ptr<cards::Card> Core::RemoveCardFromActiveCards(int cardIndex, int playerIndex)
{
    std::unique_ptr<cards::Card> movedCard = std::move(_state.players[playerIndex].activeCards[cardIndex]);
    _state.players[playerIndex].activeCards.erase(_state.players[playerIndex].activeCards.begin() + cardIndex);

    CardLeaveActivesEvent event;
    event.card = movedCard.get();
    event.playerIndex = playerIndex;
    _events.RaiseEvent(event);

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

    CardEnterDeckEvent event;
    event.card = deckRef.back().get();
    _events.RaiseEvent(event);

    return true;
}

std::unique_ptr<cards::Card> Core::RemoveCardFromDeck(cards::Card* card)
{
    auto& deckRef = _ResolveDeckFromType(card->GetCardType());
    for (int i = 0; i < deckRef.size(); i++)
    {
        if (deckRef[i].get() == card)
        {
            return RemoveCardFromDeck(card->GetCardType(), i);
        }
    }
    return nullptr;
}

std::unique_ptr<cards::Card> Core::RemoveCardFromDeck(cards::CardType deck, int cardIndex)
{
    auto& deckRef = _ResolveDeckFromType(deck);
    std::unique_ptr<cards::Card> movedCard = std::move(deckRef[cardIndex]);
    deckRef.erase(deckRef.begin() + cardIndex);

    CardLeaveDeckEvent event;
    event.card = movedCard.get();
    _events.RaiseEvent(event);

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

    CardEnterGraveyardEvent event;
    event.card = _state.graveyard.back().get();
    _events.RaiseEvent(event);
}

std::unique_ptr<cards::Card> Core::RemoveCardFromGraveyard(cards::Card* card)
{
    for (int i = 0; i < _state.graveyard.size(); i++)
    {
        if (_state.graveyard[i].get() == card)
        {
            return RemoveCardFromGraveyard(i);
        }
    }
    return nullptr;
}

std::unique_ptr<cards::Card> Core::RemoveCardFromGraveyard(int cardIndex)
{
    std::unique_ptr<cards::Card> movedCard = std::move(_state.graveyard[cardIndex]);
    _state.graveyard.erase(_state.graveyard.begin() + cardIndex);

    CardLeaveGraveyardEvent event;
    event.card = movedCard.get();
    _events.RaiseEvent(event);

    return movedCard;
}

void Core::AddCardToInPlayCards(std::unique_ptr<cards::Card> card)
{
    if (card->IsVirtual() && !card->IsAllowedSet(cards::CardSet{ cards::CardSets::IN_PLAY }))
    {
        AddCardToDestroyedCards(std::move(card));
        return;
    }

    _state.inPlayCards.push_back(std::move(card));

    CardEnterInPlayCardsEvent event;
    event.card = _state.inPlayCards.back().get();
    _events.RaiseEvent(event);
}

std::unique_ptr<cards::Card> Core::RemoveCardFromInPlayCards(cards::Card* card)
{
    for (int i = 0; i < _state.inPlayCards.size(); i++)
    {
        if (_state.inPlayCards[i].get() == card)
        {
            return RemoveCardFromInPlayCards(i);
        }
    }
    return nullptr;
}

std::unique_ptr<cards::Card> Core::RemoveCardFromInPlayCards(int cardIndex)
{
    std::unique_ptr<cards::Card> movedCard = std::move(_state.inPlayCards[cardIndex]);
    _state.inPlayCards.erase(_state.inPlayCards.begin() + cardIndex);

    CardLeaveInPlayCardsEvent event;
    event.card = movedCard.get();
    _events.RaiseEvent(event);

    return movedCard;
}

void Core::AddCardToDestroyedCards(std::unique_ptr<cards::Card> card)
{
    card->OnEnterDestroyedCards(this);
    _state.destroyedCards.push_back(std::move(card));

    CardEnterDestroyedCardsEvent event;
    event.card = _state.destroyedCards.back().get();
    _events.RaiseEvent(event);
}

std::unique_ptr<cards::Card> Core::RemoveCardFromDestroyedCards(cards::Card* card)
{
    for (int i = 0; i < _state.destroyedCards.size(); i++)
    {
        if (_state.destroyedCards[i].get() == card)
        {
            return RemoveCardFromDestroyedCards(i);
        }
    }
    return nullptr;
}

std::unique_ptr<cards::Card> Core::RemoveCardFromDestroyedCards(int cardIndex)
{
    std::unique_ptr<cards::Card> movedCard = std::move(_state.destroyedCards[cardIndex]);
    _state.destroyedCards.erase(_state.destroyedCards.begin() + cardIndex);

    CardLeaveDestroyedCardsEvent event;
    event.card = movedCard.get();
    _events.RaiseEvent(event);

    return movedCard;
}

void Core::AddCardToDisplayedCards(DisplayInfo displayInfo)
{
    _state.displayedCards.push_back(displayInfo);

    CardEnterDisplayedCardsEvent event;
    event.displayInfo = _state.displayedCards.back();
    _events.RaiseEvent(event);
}

bool Core::RemoveCardFromDisplayedCards(cards::Card* card)
{
    for (int i = 0; i < _state.displayedCards.size(); i++)
    {
        if (_state.displayedCards[i].card == card)
        {
            RemoveCardFromDisplayedCards(i);
            return true;
        }
    }
    return false;
}

void Core::RemoveCardFromDisplayedCards(int cardIndex)
{
    cards::Card* card = _state.displayedCards[cardIndex].card;

    _state.displayedCards.erase(_state.displayedCards.begin() + cardIndex);

    CardLeaveDisplayedCardsEvent event;
    event.card = card;
    _events.RaiseEvent(event);
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
        cards::Card* card = _state.displayedCards.front().card;

        _state.displayedCards.erase(_state.displayedCards.begin());

        CardLeaveDisplayedCardsEvent event;
        event.card = card;
        _events.RaiseEvent(event);
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
    std::mt19937 engineCopy = _rng;

    auto& deckRef = _ResolveDeckFromType(type);
    std::shuffle(deckRef.begin(), deckRef.end(), _rng);

    _events.RaiseEvent(DeckShuffledEvent{ type, engineCopy });
}

int Core::GenerateRandomNumber(int fromInclusive, int toExclusive)
{
    auto dist = std::uniform_int_distribution<int>(fromInclusive, toExclusive - 1);
    return dist(_rng);
}

void Core::RevealHand(int target, std::string revealSource)
{
    if (!_state.players[target].handRevealed)
        _events.RaiseEvent(HandRevealStateChangedEvent{ target, true });
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
    {
        if (_state.players[target].handRevealed)
            _events.RaiseEvent(HandRevealStateChangedEvent{ target, false });
        _state.players[target].handRevealed = false;
    }
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

std::vector<cards::Card*> Core::GetRegisteredCards()
{
    std::vector<cards::Card*> cards;
    for (auto& card : _registeredCards)
        cards.push_back(card.get());
    return cards;
}

int Core::OpponentOf(int playerIndex)
{
    if (playerIndex == 0)
        return 1;
    else if (playerIndex == 1)
        return 0;
    else
        return -1;
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