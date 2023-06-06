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

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Hidden Treasures"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}