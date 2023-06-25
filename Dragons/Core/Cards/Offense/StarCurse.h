#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class StarCurse : public Card
    {
    public:
        StarCurse() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "star_curse" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new StarCurse()); }

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Star Curse"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}