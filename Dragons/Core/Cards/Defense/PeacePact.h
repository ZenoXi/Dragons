#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class PeacePact : public Card
    {
    public:
        PeacePact() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "peace_pact" }; }
        CardId GetCardId() const { return CARD_ID(); }

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Peace Pact"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}