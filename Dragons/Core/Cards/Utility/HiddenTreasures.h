#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class HiddenTreasures : public Card
    {
        bool _waitingForCardDraw = false;

    public:
        HiddenTreasures() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "hidden_treasures" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new HiddenTreasures()); }

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Hidden Treasures"; }
        std::wstring GetCardDescription() const { return L"Draw 2 cards."; }
        std::wstring GetCardFlavorText() const { return L"Being a good treasure hunter can put you at risk"; }
    };
}