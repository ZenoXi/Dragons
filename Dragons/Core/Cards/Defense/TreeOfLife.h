#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    struct TreeOfLifePlayProperties : public PlayProperties
    {
        bool healOpponent = true;
    };

    class TreeOfLife : public Card
    {
    public:
        TreeOfLife() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "tree_of_life" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new TreeOfLife()); }

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Tree of Life"; }
        std::wstring GetCardDescription() const { return L"Heal 4 health to both players."; }
        std::wstring GetCardFlavorText() const { return L"Everyone is equal before the power of nature"; }
    };
}