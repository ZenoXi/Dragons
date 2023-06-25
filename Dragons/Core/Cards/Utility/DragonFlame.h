#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class DragonFlame : public Card
    {
    public:
        DragonFlame() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "dragon_flame" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new DragonFlame()); }

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Dragon Flame"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}