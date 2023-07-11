#include "Shackling.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::Shackling::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::Shackling::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == DevilsDeal::CARD_ID())
            _cardDevilsDeal = card;
        else if (card->GetCardId() == Recreation::CARD_ID())
            _cardRecreation = card;
        else if (card->GetCardId() == DragonFlame::CARD_ID())
            _cardDragonFlame = card;
    }

    // Play Dragon Flame
    _cardDragonFlame->Play(core, actionProps, nullptr);

    // Play Devils Deal
    PlayResult result = _cardDevilsDeal->Play(core, actionProps, nullptr);
    if (result.waitForInput)
    {
        _resumeDevilsDeal = true;
        return result;
    }

    _resumeToRecreation = true;
    return PlayResult::Resume();
}

cards::PlayResult cards::Shackling::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_resumeDevilsDeal)
    {
        _resumeDevilsDeal = false;

        PlayResult result = _cardDevilsDeal->Resume(std::move(response), core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeDevilsDeal = true;
            return result;
        }

        _resumeToRecreation = true;
        return PlayResult::Resume();
    }
    else if (_resumeToRecreation)
    {
        _resumeToRecreation = false;

        _recreationActivated = false;
        // Recreation, if played after CanPlay returns false, will only draw cards
        if (_cardRecreation->CanPlay(core, actionProps, nullptr))
            _recreationActivated = true;
        _cardRecreation->Play(core, actionProps, nullptr);

        _resumeToCleanUp = true;
        return PlayResult::Resume();
    }
    else if (_resumeToCleanUp)
    {
        _resumeToCleanUp = false;

        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardDevilsDeal));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardDragonFlame));

        if (_recreationActivated)
        {
            core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardRecreation));
            return PlayResult::Default();
        }
        else
        {
            _targetPlayer = actionProps.opponent;
            core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardRecreation), actionProps.player);
            return PlayResult::AddToActives();
        }
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}

void cards::Shackling::_OnEnterHand(Core* core, int playerIndex)
{
    _canPlayHandler.reset();
    _preDamageHandler.reset();
    _cardLeaveDeckHandler.reset();
}

void cards::Shackling::_OnEnterActiveCards(Core* core, int playerIndex)
{
    _canPlayHandler = std::make_unique<EventHandler<CanPlayEvent>>(&core->Events(), [=](CanPlayEvent event)
    {
        if (event.actionProps->opponent != GetPosition().playerIndex)
            return;

        if (event.card->IsActive())
            *event.canPlay = false;
    });
    _preDamageHandler = std::make_unique<EventHandler<PreDamageEvent_BuffPass>>(&core->Events(), [=](PreDamageEvent_BuffPass event)
    {
        if (event.props->source != _targetPlayer || (event.props->sourceCard && event.props->sourceCard->GetCardType() != CardType::OFFENSE))
            return;

        if (event.props->target != _targetPlayer)
            event.props->target = _targetPlayer;
    });
    _cardLeaveDeckHandler = std::make_unique<EventHandler<CardLeaveDeckEvent>>(&core->Events(), [=](CardLeaveDeckEvent event)
    {
        if (event.card->GetCardType() != CardType::DEFENSE || !core->GetState().defenseDeck.empty())
            return;

        auto cardPtr = core->RemoveCardFromActiveCards(_cardRecreation, GetPosition().playerIndex);
        if (cardPtr)
        {
            cardPtr->Play(core, ActionProperties{}, nullptr);
            core->AddCardToGraveyard(std::move(cardPtr));
        }

        core->AddCardToGraveyard(core->RemoveCardFromActiveCards(this, GetPosition().playerIndex));
    });
}

void cards::Shackling::_OnEnterDeck(Core* core)
{
    _canPlayHandler.reset();
    _preDamageHandler.reset();
    _cardLeaveDeckHandler.reset();
}

void cards::Shackling::_OnEnterGraveyard(Core* core)
{
    _canPlayHandler.reset();
    _preDamageHandler.reset();
    _cardLeaveDeckHandler.reset();
}

void cards::Shackling::_OnEnterDestroyedCards(Core* core)
{
    _canPlayHandler.reset();
    _preDamageHandler.reset();
    _cardLeaveDeckHandler.reset();
}