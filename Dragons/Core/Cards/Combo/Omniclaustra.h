#pragma once

#include "../Card.h"
#include "../Defense/SoothingSpell.h"
#include "../Defense/DivineProtection.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/StatsEvents.h"

class Core;

namespace cards
{
    class Omniclaustra : public Card
    {
        bool _resumeFireMoon = false;
        bool _resumeToSoothingSpell = false;

        std::unique_ptr<EventHandler<PreHealthChangeEvent>> _preHealthChangeHandler = nullptr;

        const std::vector<CardId> _requiredCardIds
        {
            SoothingSpell::CARD_ID(),
            DivineProtection::CARD_ID()
        };
        cards::Card* _cardSoothingSpell = nullptr;
        cards::Card* _cardDivineProtection = nullptr;

    public:
        Omniclaustra() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        bool IsActive() { return true; }

        static CardId CARD_ID() { return { "omniclaustra" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new Omniclaustra()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Omniclaustra"; }
        std::wstring GetCardDescription() const { return L""; }

    private:
        void _OnEnterHand(Core* core, int playerIndex);
        void _OnEnterActiveCards(Core* core, int playerIndex);
        void _OnEnterDeck(Core* core);
        void _OnEnterGraveyard(Core* core);
        void _OnEnterDestroyedCards(Core* core);
    };
}