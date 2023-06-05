#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class DragonSight : public Card
    {
        bool _waitingForDeckChoice = false;
        bool _waitingForConfirmation = false;

    public:
        DragonSight() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Dragon Sight"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}