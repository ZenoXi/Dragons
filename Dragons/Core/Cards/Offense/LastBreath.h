#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class LastBreath : public Card
    {
        bool _waitingToPlayCards = false;

    public:
        LastBreath() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);
        int GetActionCost() { return 2; }

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Last Breath"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}