#pragma once

#include "../Card.h"
#include "../Offense/DevilsDeal.h"
#include "../Defense/Recreation.h"
#include "../Utility/DragonFlame.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/StatsEvents.h"
#include "../../Events/ActionEvents.h"
#include "../../Events/CardStateEvents.h"

class Core;

namespace cards
{
    class Shackling : public Card
    {
        bool _resumeDevilsDeal = false;
        bool _resumeToRecreation = false;
        bool _resumeToCleanUp = false;

        bool _recreationActivated = false;
        std::unique_ptr<EventHandler<CanPlayEvent>> _canPlayHandler = nullptr;
        std::unique_ptr<EventHandler<PreDamageEvent_BuffPass>> _preDamageHandler = nullptr;
        std::unique_ptr<EventHandler<CardLeaveDeckEvent>> _cardLeaveDeckHandler = nullptr;
        int _targetPlayer = -1;

        const std::vector<CardId> _requiredCardIds
        {
            DevilsDeal::CARD_ID(),
            Recreation::CARD_ID(),
            DragonFlame::CARD_ID()
        };
        cards::Card* _cardDevilsDeal = nullptr;
        cards::Card* _cardRecreation = nullptr;
        cards::Card* _cardDragonFlame = nullptr;

    public:
        Shackling() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);
        bool IsActive() { return true; }

        static CardId CARD_ID() { return { "shackling" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new Shackling()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Shackling"; }
        std::wstring GetCardDescription() const { return L""; }

    private:
        void _OnEnterHand(Core* core, int playerIndex);
        void _OnEnterActiveCards(Core* core, int playerIndex);
        void _OnEnterDeck(Core* core);
        void _OnEnterGraveyard(Core* core);
        void _OnEnterDestroyedCards(Core* core);
    };
}