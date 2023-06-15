#include "DeathFee.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::DeathFee::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::DeathFee::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == DeathPoison::CARD_ID())
            _cardDeathPoison = std::move(card);
        else if (card->GetCardId() == HelpingHand::CARD_ID())
            _cardHelpingHand = std::move(card);
        else if (card->GetCardId() == SummonDead::CARD_ID())
            _cardSummonDead = std::move(card);
    }
    cardsForCombo.clear();

    // Play Death Poison
    _cardDeathPoison->Play(core, actionProps, playProps);

    // Play Summon Dead
    if (_cardSummonDead->CanPlay(core, actionProps, playProps))
    {
        PlayResult result = _cardSummonDead->Play(core, actionProps, playProps);
        if (result.waitForInput)
        {
            _resumeSummonDead = true;
            return result;
        }
    }

    _resumeToHelpingHand = true;
    return PlayResult::Resume();
}

cards::PlayResult cards::DeathFee::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_resumeSummonDead)
    {
        _cardSummonDead->Resume(std::move(response), core, actionProps, playProps);
    }

    if (_waitingToSelectCard)
    {
        _waitingToSelectCard = false;

        UserInputParams_ChooseCardFromHand* params = reinterpret_cast<UserInputParams_ChooseCardFromHand*>(response.inputParams.get());
        if (!params)
            return PlayResult::Default();
        if (params->chosenCards.size() != 1)
            return PlayResult::Default();

        auto cardPtr = core->RemoveCardFromHand(params->chosenCards[0], actionProps.player);
        core->AddCardToHand(std::move(cardPtr), actionProps.opponent);
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}