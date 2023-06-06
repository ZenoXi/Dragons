#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class Recreation : public Card
    {
    public:
        Recreation() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "recreation" }; }
        CardId GetCardId() const { return CARD_ID(); }

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Recreation"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}