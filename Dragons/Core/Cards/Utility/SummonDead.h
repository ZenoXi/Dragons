#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class SummonDead : public Card
    {
        bool _waitingForCardChoice = false;

    public:
        SummonDead() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Summon Dead"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}