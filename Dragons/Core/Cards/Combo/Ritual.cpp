#include "Ritual.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::Ritual::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::Ritual::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == LastBreath::CARD_ID())
            _cardLastBreath = card;
        else if (card->GetCardId() == LifeFlower::CARD_ID())
            _cardLifeFlower = card;
        else if (card->GetCardId() == LifeExchange::CARD_ID())
            _cardLifeExchange = card;
    }

    if (_cardLifeExchange->CanPlay(core, actionProps, nullptr))
    {
        PlayResult result = _cardLifeExchange->Play(core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeLifeExchange = true;
            return result;
        }
    }

    _resumeToLastBreath = true;
    return PlayResult::Resume();
}

cards::PlayResult cards::Ritual::Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (_resumeLifeExchange)
    {
        _resumeLifeExchange = false;

        PlayResult result = _cardLifeExchange->Resume(std::move(response), core, actionProps, nullptr);
        if (result.waitForInput)
        {
            _resumeLifeExchange = true;
            return result;
        }

        _resumeToLastBreath = true;
        return PlayResult::Resume();
    }
    else if (_resumeToLastBreath)
    {
        _resumeToLastBreath = false;

        _cardsPlayed = 0;
        if (_cardLastBreath->CanPlay(core, actionProps, nullptr))
        {
            _postCardPlayedHandler = std::make_unique<EventHandler<PostCardPlayedEvent>>(&core->Events(), [=](PostCardPlayedEvent event)
            {
                _cardsPlayed++;
            });

            LastBreathPlayProperties lastBreathPlayProperties;
            lastBreathPlayProperties.discardHand = false;
            PlayResult result = _cardLastBreath->Play(core, actionProps, &lastBreathPlayProperties);
            if (result.waitForInput)
            {
                _resumeLastBreath = true;
                return result;
            }
        }

        _resumeToLifeFlower = true;
        return PlayResult::Resume();
    }
    else if (_resumeLastBreath)
    {
        _resumeLastBreath = false;

        LastBreathPlayProperties lastBreathPlayProperties;
        lastBreathPlayProperties.discardHand = false;
        PlayResult result = _cardLastBreath->Play(core, actionProps, &lastBreathPlayProperties);
        if (result.waitForInput)
        {
            _resumeLastBreath = true;
            return result;
        }

        _resumeToLifeFlower = true;
        return PlayResult::Resume();
    }
    else if (_resumeToLifeFlower)
    {
        _resumeToLifeFlower = false;

        _postCardPlayedHandler.reset();
        for (int i = 0; i < _cardsPlayed; i++)
        {
            _cardLifeFlower->Play(core, actionProps, nullptr);
        }

        _resumeToCleanUp = true;
    }
    else if (_resumeToCleanUp)
    {
        _resumeToCleanUp = false;

        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardLastBreath));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardLifeFlower));
        core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardLifeExchange));

        return PlayResult::Default();
    }

    PlayResult result;
    result.notPlayed = true;
    return result;
}