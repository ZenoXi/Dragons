#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class LifeExchange : public Card
    {
        bool _waitingForCardDraw = false;
        bool _waitingForCardChoice = false;

    public:
        LifeExchange() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "life_exchange" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new LifeExchange()); }

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Life Exchange"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}