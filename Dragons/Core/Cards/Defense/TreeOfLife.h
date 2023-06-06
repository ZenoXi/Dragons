#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class TreeOfLife : public Card
    {
    public:
        TreeOfLife() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "tree_of_life" }; }
        CardId GetCardId() const { return CARD_ID(); }

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Tree of Life"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}