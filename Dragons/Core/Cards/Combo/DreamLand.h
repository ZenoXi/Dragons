#pragma once

#include "../Card.h"
#include "../Offense/VitalSpot.h"
#include "../Defense/SpiritShelter.h"
#include "../Utility/SetUp.h"

class Core;

namespace cards
{
    class DreamLand : public Card
    {
        const std::vector<CardId> _requiredCardIds
        {
            VitalSpot::CARD_ID(),
            SpiritShelter::CARD_ID(),
            SetUp::CARD_ID()
        };
        cards::Card* _cardVitalSpot = nullptr;
        cards::Card* _cardSpiritShelter = nullptr;
        cards::Card* _cardSetUp = nullptr;

    public:
        DreamLand() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "dream_land" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new DreamLand()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Dream Land"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}