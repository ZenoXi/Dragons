#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class Cure : public Card
    {
    public:
        Cure() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "cure" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new Cure()); }

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Cure"; }
        std::wstring GetCardDescription() const { return L"Heal half of your missing health."; }
        std::wstring GetCardFlavorText() const { return L"Not every wound can be healed back"; }
    };
}