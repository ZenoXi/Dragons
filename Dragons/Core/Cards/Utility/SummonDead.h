#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class SummonDead : public Card
    {
        bool _waitingForCardChoice = false;

    public:
        SummonDead() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "summon_dead" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new SummonDead()); }

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Summon Dead"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}