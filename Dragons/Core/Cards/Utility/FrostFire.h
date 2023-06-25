#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class FrostFire : public Card
    {
        bool _waitingForDeckChoice = false;

    public:
        FrostFire() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "frost_fire" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new FrostFire()); }

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Frost Fire"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}