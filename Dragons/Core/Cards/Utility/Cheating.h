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

        static CardId CARD_ID() { return { "cheating" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new Cheating()); }

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Cheating"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}