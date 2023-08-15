#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    struct PeacePactPlayProperties : public PlayProperties
    {
        bool discardSelf = true;
    };

    class PeacePact : public Card
    {
    public:
        PeacePact() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "peace_pact" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new PeacePact()); }

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Peace Pact"; }
        std::wstring GetCardDescription() const { return L"Both players reshuffle their offensive cards into the deck."; }
        std::wstring GetCardFlavorText() const { return L"World is better in peace"; }
    };
}