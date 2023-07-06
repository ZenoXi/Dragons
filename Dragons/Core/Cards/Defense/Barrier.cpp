#include "Barrier.h"

#include "../../Core.h"

cards::PlayResult cards::Barrier::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto playPropsValue = GetPlayProperties<BarrierPlayProperties>(playProps);
    _attacksToBlock = playPropsValue.attacksToBlock;
    _blockDragonFlame = playPropsValue.blockDragonFlame;
    _dragonFlameBlockPlayProps.blocked = true;

    return PlayResult::AddToActives();
}

void cards::Barrier::_OnEnterHand(Core* core, int playerIndex)
{
    _preDamageHandler.reset();
    _preCardPlayedHandler.reset();
}

void cards::Barrier::_OnEnterActiveCards(Core* core, int playerIndex)
{
    _preDamageHandler = std::make_unique<EventHandler<PreDamageEvent_NerfPass>>(&core->Events(), [=](PreDamageEvent_NerfPass event)
    {
        if (GetPosition().playerIndex != event.props->target)
            return;
        if (event.props->trueDamage || event.props->fatigue)
            return;
        if (event.props->amount == 0)
            return;

        event.props->amount = 0;

        if (--_attacksToBlock <= 0)
        {
            auto cardPtr = core->RemoveCardFromActiveCards(this, GetPosition().playerIndex);
            core->AddCardToGraveyard(std::move(cardPtr));
        }
    });

    _preCardPlayedHandler = std::make_unique<EventHandler<PreCardPlayedEvent>>(&core->Events(), [=](PreCardPlayedEvent event)
    {
        if (GetPosition().playerIndex != event.actionProps->opponent)
            return;

        if (event.card->GetCardId() == DragonFlame::CARD_ID() && _blockDragonFlame)
        {
            if (*event.playProps)
                GetPlayPropertiesPtr<DragonFlamePlayProps>(*event.playProps)->blocked = true;
            else
                *event.playProps = &_dragonFlameBlockPlayProps;

            if (--_attacksToBlock <= 0)
            {
                auto cardPtr = core->RemoveCardFromActiveCards(this, GetPosition().playerIndex);
                core->AddCardToGraveyard(std::move(cardPtr));
            }
        }
    });
}

void cards::Barrier::_OnEnterDeck(Core* core)
{
    _preDamageHandler.reset();
    _preCardPlayedHandler.reset();
}

void cards::Barrier::_OnEnterGraveyard(Core* core)
{
    _preDamageHandler.reset();
    _preCardPlayedHandler.reset();
}

void cards::Barrier::_OnEnterDestroyedCards(Core* core)
{
    _preDamageHandler.reset();
    _preCardPlayedHandler.reset();
}