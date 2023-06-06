#include "DragonPower.h"

#include "../../Core.h"

bool cards::DragonPower::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->GetState().offenseDeck.empty()
        || core->GetState().defenseDeck.empty()
        || core->GetState().utilityDeck.empty()
        || core->GetState().comboDeck.empty();
}

cards::PlayResult cards::DragonPower::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    int damageAmount = 0;
    int healAmount = 0;
    int cardsToDraw = 0;

    if (core->GetState().offenseDeck.empty())
    {
        damageAmount++;
    }
    if (core->GetState().defenseDeck.empty())
    {
        healAmount++;
    }
    if (core->GetState().utilityDeck.empty())
    {
        cardsToDraw++;
    }
    if (core->GetState().comboDeck.empty())
    {
        damageAmount++;
        healAmount++;
        cardsToDraw++;
    }

    if (damageAmount > 0)
    {
        DamageProperties damageProps;
        damageProps.source = actionProps.player;
        damageProps.target = actionProps.opponent;
        damageProps.amount = damageAmount;
        core->Damage(damageProps);
    }
    if (healAmount > 0)
    {
        core->Heal(actionProps.player, healAmount);
    }
    if (cardsToDraw > 0)
    {
        auto params = std::make_unique<UserInputParams_DrawCard>();
        params->minCardCount = cardsToDraw;
        params->maxCardCount = cardsToDraw;
        params->playerIndex = actionProps.player;

        _waitingForCardDraw = true;

        PlayResult result;
        result.waitForInput = true;
        result.inputRequest.inputType = UserInputType::DRAW_CARD;
        result.inputRequest.inputPrompt = L"Draw a card";
        if (cardsToDraw > 1)
            result.inputRequest.inputPrompt = L"Draw cards";
        result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
        return result;
    }

    return PlayResult::Default();
}

cards::PlayResult cards::DragonPower::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_waitingForCardDraw)
    {
        UserInputParams_DrawCard* responseParams = reinterpret_cast<UserInputParams_DrawCard*>(response.inputParams.get());
        if (!responseParams)
            return PlayResult::Default();

        _waitingForCardDraw = false;
        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}