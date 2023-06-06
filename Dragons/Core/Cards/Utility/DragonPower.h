#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class DragonPower : public Card
    {
        bool _waitingForCardDraw = false;

    public:
        DragonPower() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Dragon Power"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}