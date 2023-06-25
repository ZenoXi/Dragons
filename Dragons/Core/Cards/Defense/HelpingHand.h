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

        static CardId CARD_ID() { return { "helping_hand" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new HelpingHand()); }

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Helping Hand"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}