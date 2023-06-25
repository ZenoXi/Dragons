#include "UltimateDefense.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::UltimateDefense::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::UltimateDefense::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == WarBlessing::CARD_ID())
            _cardWarBlessing = card;
        else if (card->GetCardId() == Barrier::CARD_ID())
            _cardBarrier = card;
    }

    // Play War Blessing
    _cardWarBlessing->Play(core, actionProps, nullptr);

    // Play Barrier
    _cardBarrier->Play(core, actionProps, nullptr);
    core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardBarrier), actionProps.player);

    // Draw card
    if (core->GetState().players[actionProps.player].hand.size() >= GAME_HAND_SIZE)
    {
        _resumeToCleanUp = true;
        return PlayResult::Resume();
    }

    auto params = std::make_unique<UserInputParams_DrawCard>();
    params->minCardCount = 1;
    params->maxCardCount = 1;
    params->playerIndex = actionProps.player;

    _waitingForCardDraw = true;

    PlayResult result;
    result.waitForInput = true;
    result.inputRequest.inputType = UserInputType::DRAW_CARD;
    result.inputRequest.inputPrompt = L"Draw a card";
    result.inputRequest.inputParams = std::unique_ptr<UserInputParams>(params.release());
    return result;

}

cards::PlayResult cards::UltimateDefense::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_waitingForCardDraw)
    {
        _waitingForCardDraw = false;

        _resumeToCleanUp = true;
        return PlayResult::Resume();
    }
    else if (_resumeToCleanUp)
    {
        _resumeToCleanUp = false;

        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardWarBlessing));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardBarrier));

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}