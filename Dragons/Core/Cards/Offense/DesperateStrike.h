#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    struct DesperateStrikePlayProperties : public PlayProperties
    {
        int damageAmount = 3;
    };

    class DesperateStrike : public Card
    {
    public:
        DesperateStrike() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "desperate_strike" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new DesperateStrike()); }

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Desperate Strike"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}