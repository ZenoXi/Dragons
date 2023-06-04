#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class HelpingHand : public Card
    {
        bool _waitingToSelectCard = false;

    public:
        HelpingHand() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Helping Hand"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}