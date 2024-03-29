#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class TheThief : public Card
    {
        bool _waitingForCardChoice = false;

    public:
        TheThief() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "the_thief" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new TheThief()); }

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"The Thief"; }
        std::wstring GetCardDescription() const { return L"Take a random card from enemy."; }
        std::wstring GetCardFlavorText() const { return L"She is as rich as everyone around her"; }
    };
}