#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class Cheating : public Card
    {
        bool _waitingForConfirmation = false;

    public:
        Cheating() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Cheating"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}