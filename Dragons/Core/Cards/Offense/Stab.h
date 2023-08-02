#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class Stab : public Card
    {
        bool _waitingForChoice = false;

    public:
        Stab() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "stab" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new Stab()); }

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Stab"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}