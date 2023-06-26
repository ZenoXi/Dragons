#include "ElementalDragon.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::ElementalDragon::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::ElementalDragon::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == HeavySlash::CARD_ID())
            _cardHeavySlash = card;
        else if (card->GetCardId() == ShieldingNet::CARD_ID())
            _cardShieldingNet = card;
        else if (card->GetCardId() == CorpsePuppet::CARD_ID())
            _cardCorpsePuppet = card;
    }

    // Play Corpse Puppet
    _cardCorpsePuppet->Play(core, actionProps, nullptr);
    core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardCorpsePuppet), actionProps.player);

    // Play Heavy Slash
    _cardHeavySlash->Play(core, actionProps, nullptr);
    core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardHeavySlash), actionProps.player);

    // Play Shielding Net
    _cardShieldingNet->Play(core, actionProps, nullptr);
    core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardShieldingNet), actionProps.player);

    if (core->GetState().players[actionProps.player].CardsInHand(CardType::COMBO) == 0)
        return PlayResult::Default();

    // Get player choice for combo effect
    auto params = std::make_unique<UserInputParams_YesOrNo>();
    params->playerIndex = actionProps.player;

    _waitingForResponse = true;

    PlayResult result;
    result.waitForInput = true;
    result.inputRequest.inputType = UserInputType::YES_OR_NO;
    result.inputRequest.inputPrompt = L"Would you like to destroy your active cards to activate another combo for free?";
    result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
    return result;
}

cards::PlayResult cards::ElementalDragon::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_waitingForResponse)
    {
        _waitingForResponse = false;
        UserInputParams_YesOrNo* params = reinterpret_cast<UserInputParams_YesOrNo*>(response.inputParams.get());
        if (!params)
            return PlayResult::Default();

        // Destroy actives
        auto& activeCardRef = core->GetState().players[actionProps.player].activeCards;
        while (!activeCardRef.empty())
            core->AddCardToDestroyedCards(core->RemoveCardFromActiveCards(0, actionProps.player));

        _resumeToComboChoice = true;
        return PlayResult::Resume();
    }
    else if (_resumeToComboChoice)
    {
        _resumeToComboChoice = false;

        // Choose combo to activate
        auto params = std::make_unique<UserInputParams_ChooseCardFromHand>();
        params->choosingPlayerIndex = actionProps.player;
        params->handPlayerIndex = actionProps.player;
        params->minCardCount = 1;
        params->maxCardCount = 1;
        params->allowedTypes.push_back(CardType::COMBO);

        _waitingForComboChoice = true;

        PlayResult result;
        result.waitForInput = true;
        result.inputRequest.inputType = UserInputType::CHOOSE_CARD_FROM_HAND;
        result.inputRequest.inputPrompt = L"Choose a combo card to activate";
        result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
        return result;
    }
    else if (_waitingForComboChoice)
    {
        _waitingForComboChoice = false;
        UserInputParams_ChooseCardFromHand* params = reinterpret_cast<UserInputParams_ChooseCardFromHand*>(response.inputParams.get());
        if (!params)
            return PlayResult::Default();
        if (params->chosenCards.empty())
            return PlayResult::Default();

        _chosenCard = params->chosenCards.front();
        core->AddCardToInPlayCards(core->RemoveCardFromHand(_chosenCard, actionProps.player));

        // Create max priority combo card retrieving handler,
        // which will supply virtual cards to the next combo called
        _preGetComboCardsHandler = std::make_unique<EventHandler<PreGetComboCardsEvent>>(&core->Events(), [=](PreGetComboCardsEvent event)
        {
            // Create virtual cards
            for (auto& cardId : event.comboProps.requiredCards)
            {
                auto cardPtr = core->CreateCard(cardId);
                cardPtr->SetVirtual(true);
                cardPtr->SetAllowedSets({
                    CardSet{ CardSets::IN_PLAY },
                    CardSet{ CardSets::ACTIVE_CARDS, actionProps.player },
                    CardSet{ CardSets::ACTIVE_CARDS, actionProps.opponent },
                });

                event.retrievedCards->push_back(cardPtr.get());
                core->AddCardToInPlayCards(std::move(cardPtr));
            }
            
            *event.processed = true;
        }, true /* max priority */);

        PlayResult result = _chosenCard->Play(core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeComboPlay = true;
            return result;
        }
        else
        {
            if (result.addToActives)
                core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_chosenCard), actionProps.player);
            else if (result.discard)
                core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_chosenCard));
            else
                core->AddCardToHand(core->RemoveCardFromInPlayCards(_chosenCard), actionProps.player);
            return PlayResult::Default();
        }
    }
    else if (_resumeComboPlay)
    {
        _resumeComboPlay = false;
        PlayResult result = _chosenCard->Resume(std::move(response), core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeComboPlay = true;
            return result;
        }
        else
        {
            if (result.addToActives)
                core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_chosenCard), actionProps.player);
            else if (result.discard)
                core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_chosenCard));
            else
                core->AddCardToHand(core->RemoveCardFromInPlayCards(_chosenCard), actionProps.player);
            return PlayResult::Default();
        }
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}