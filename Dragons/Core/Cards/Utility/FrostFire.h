#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class FrostFire : public Card
    {
        bool _waitingForDeckChoice = false;

    public:
        FrostFire() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Frost Fire"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}