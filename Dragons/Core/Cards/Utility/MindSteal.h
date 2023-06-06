#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class MindSteal : public Card
    {
        bool _waitingForCardChoice = false;

    public:
        MindSteal() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Mind Steal"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}