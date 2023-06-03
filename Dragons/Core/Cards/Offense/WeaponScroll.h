#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class WeaponScroll : public Card
    {
        bool _waitingForCardChoice = false;

    public:
        WeaponScroll() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Weapon Scroll"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}