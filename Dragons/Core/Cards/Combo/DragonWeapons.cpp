#include "DragonWeapons.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::DragonWeapons::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::DragonWeapons::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == DragonSight::CARD_ID())
            _cardDragonSight = card;
        else if (card->GetCardId() == WeaponOfChoice::CARD_ID())
            _cardWeaponOfChoice = card;
    }

    // Play Dragon Sight
    PlayResult result = _cardDragonSight->Play(core, actionProps, nullptr);
    if (result.waitForInput)
    {
        _resumeDragonSight = true;
        return result;
    }

    _resumeToWeaponOfChoice = true;
    return PlayResult::Resume();
}

cards::PlayResult cards::DragonWeapons::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_resumeDragonSight)
    {
        _resumeDragonSight = false;

        PlayResult result = _cardDragonSight->Resume(std::move(response), core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeDragonSight = true;
            return result;
        }

        _resumeToWeaponOfChoice = true;
        return PlayResult::Resume();
    }
    else if (_resumeToWeaponOfChoice)
    {
        _resumeToWeaponOfChoice = false;

        int freeSlotsInHand = GAME_HAND_SIZE - core->GetState().players[actionProps.player].hand.size();
        if (freeSlotsInHand <= 0)
        {
            _resumeToCleanUp = true;
            return PlayResult::Resume();
        }
        freeSlotsInHand -= 1; // Combo deck is always viewed

        auto params = std::make_unique<UserInputParams_ChooseDeck>();
        params->playerIndex = actionProps.player;
        params->minDeckCount = 0;
        params->maxDeckCount = freeSlotsInHand > 3 ? 3 : freeSlotsInHand;
        params->allowEmptyDecks = false;
        params->allowedDecks.push_back(CardType::OFFENSE);
        params->allowedDecks.push_back(CardType::DEFENSE);
        params->allowedDecks.push_back(CardType::UTILITY);

        _waitingForDeckChoice = true;

        PlayResult result;
        result.waitForInput = true;
        result.inputRequest.inputType = UserInputType::CHOOSE_DECK;
        result.inputRequest.inputPrompt = L"Select additional decks to which Weapon of Choice will be applied to";
        result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
        return result;
    }
    else if (_waitingForDeckChoice)
    {
        _waitingForDeckChoice = false;

        UserInputParams_ChooseDeck* responseParams = reinterpret_cast<UserInputParams_ChooseDeck*>(response.inputParams.get());
        if (!responseParams)
            return PlayResult::Default();

        _decksToView = responseParams->chosenDecks;
        if (!core->GetState().comboDeck.empty())
            _decksToView.push_back(CardType::COMBO);

        _resumeToWeaponOfChoiceEffect = true;
        return PlayResult::Resume();
    }
    else if (_resumeToWeaponOfChoiceEffect)
    {
        _resumeToWeaponOfChoiceEffect = false;

        if (_decksToView.empty())
        {
            _resumeToCleanUp = true;
            return PlayResult::Resume();
        }

        WeaponOfChoicePlayProperties weaponOfChoicePlayProperties;
        weaponOfChoicePlayProperties.deck = _decksToView.front();
        PlayResult result = _cardWeaponOfChoice->Play(core, actionProps, &weaponOfChoicePlayProperties);
        if (result.waitForInput)
        {
            _resumeWeaponOfChoiceEffect = true;
            return result;
        }

        _decksToView.erase(_decksToView.begin());
        _resumeToWeaponOfChoiceEffect = true;
        return PlayResult::Resume();
    }
    else if (_resumeWeaponOfChoiceEffect)
    {
        _resumeWeaponOfChoiceEffect = false;

        WeaponOfChoicePlayProperties weaponOfChoicePlayProperties;
        weaponOfChoicePlayProperties.deck = _decksToView.front();
        PlayResult result = _cardWeaponOfChoice->Resume(std::move(response), core, actionProps, &weaponOfChoicePlayProperties);
        if (result.waitForInput)
        {
            _resumeWeaponOfChoiceEffect = true;
            return result;
        }

        _decksToView.erase(_decksToView.begin());
        _resumeToWeaponOfChoiceEffect = true;
        return PlayResult::Resume();
    }
    else if (_resumeToCleanUp)
    {
        _resumeToCleanUp = false;

        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardDragonSight));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardWeaponOfChoice));

        return PlayResult::Default();
    }

    return PlayResult::Default();



    PlayResult result;
    result.notPlayed = true;
    return result;
}