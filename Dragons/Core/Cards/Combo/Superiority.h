#pragma once

#include "../Card.h"
#include "../Offense/HellfireSword.h"
#include "../Defense/ArmorUp.h"
#include "../Utility/Cheating.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/StatsEvents.h"

class Core;

namespace cards
{
    class Superiority : public Card
    {
        std::unique_ptr<EventHandler<PostArmorChangeEvent>> _postArmorChangeHandler = nullptr;
        int _revealedHandIndex = -1;

        const std::vector<CardId> _requiredCardIds
        {
            HellfireSword::CARD_ID(),
            ArmorUp::CARD_ID(),
            Cheating::CARD_ID()
        };
        cards::Card* _cardHellfireSword = nullptr;
        cards::Card* _cardArmorUp = nullptr;
        cards::Card* _cardCheating = nullptr;

    public:
        Superiority() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        bool IsActive() { return true; }

        static CardId CARD_ID() { return { "superiority" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new Superiority()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Superiority"; }
        std::wstring GetCardDescription() const { return L""; }

    private:
        void _OnEnterHand(Core* core, int playerIndex);
        void _OnEnterActiveCards(Core* core, int playerIndex);
        void _OnEnterDeck(Core* core);
        void _OnEnterGraveyard(Core* core);
        void _OnEnterDestroyedCards(Core* core);
    };
}