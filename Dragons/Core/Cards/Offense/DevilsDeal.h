#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class DevilsDeal : public Card
    {
        bool _waitingForCardChoice = false;

    public:
        DevilsDeal() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "devils_deal" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new DevilsDeal()); }

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Devils Deal"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}