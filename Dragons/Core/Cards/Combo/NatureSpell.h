#pragma once

#include "../Card.h"
#include "../Defense/Cure.h"
#include "../Defense/LifeFlower.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/StatsEvents.h"

class Core;

namespace cards
{
    class NatureSpell : public Card
    {
        std::unique_ptr<EventHandler<PostHealEvent>> _postHealHandler = nullptr;

        const std::vector<CardId> _requiredCardIds
        {
            Cure::CARD_ID(),
            LifeFlower::CARD_ID()
        };
        cards::Card* _cardCure = nullptr;
        cards::Card* _cardLifeFlower = nullptr;

    public:
        NatureSpell() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "nature_spell" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new NatureSpell()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Nature Spell"; }
        std::wstring GetCardDescription() const { return L"Cure effect\nLife Flower effect\nLife Flower armor and maximum health are increased to cure power."; }
        std::wstring GetCardFlavorText() const { return L"As powerful as you let it grow"; }
    };
}