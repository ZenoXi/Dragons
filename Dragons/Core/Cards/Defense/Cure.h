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

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Cure"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}