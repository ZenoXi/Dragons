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

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Dragon Flame"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}