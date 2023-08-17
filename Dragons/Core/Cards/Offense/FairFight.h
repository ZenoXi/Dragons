#pragma once

#include "../Card.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/TurnEvents.h"

#include <memory>

class Core;

namespace cards
{
    struct FairFightPlayProperties : public PlayProperties
    {
        bool giveOpponentArmor = true;
    };

    class FairFight : public Card
    {
        std::unique_ptr<EventHandler<TurnEndEvent>> _turnEndHandler = nullptr;
        _RelativeTarget _player = _RelativeTarget::OWNER;
        _RelativeTarget _opponent = _RelativeTarget::OPPONENT;

    public:
        FairFight() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        bool IsActive() { return true; }

        static CardId CARD_ID() { return { "fair_fight" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new FairFight()); }

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Fair Fight"; }
        std::wstring GetCardDescription() const { return L"Give your opponent 4 armor. At the end of your turn deal 4 damage."; }
        std::wstring GetCardFlavorText() const { return L"It's a seemingly fair fight"; }

    private:
        void _OnEnterHand(Core* core, int playerIndex);
        void _OnEnterActiveCards(Core* core, int playerIndex);
        void _OnEnterDeck(Core* core);
        void _OnEnterGraveyard(Core* core);
        void _OnEnterDestroyedCards(Core* core);
    };
}