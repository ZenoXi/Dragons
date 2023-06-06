#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class LifeExchange : public Card
    {
        bool _waitingForCardDraw = false;
        bool _waitingForCardChoice = false;

    public:
        LifeExchange() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Life Exchange"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}