#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class Preparations : public Card
    {
    public:
        Preparations() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "preparations" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new Preparations()); }

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Preparations"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}