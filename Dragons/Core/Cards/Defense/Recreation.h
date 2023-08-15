#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class Recreation : public Card
    {
    public:
        Recreation() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "recreation" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new Recreation()); }

        CardType GetCardType() const { return CardType::DEFENSE; }
        std::wstring GetCardName() const { return L"Recreation"; }
        std::wstring GetCardDescription() const { return L"If the defensive deck is empty, create a new one from defensive cards in the graveyard, draw two defensive cards."; }
        std::wstring GetCardFlavorText() const { return L"A chamber with the power to recreate a whole city"; }
    };
}