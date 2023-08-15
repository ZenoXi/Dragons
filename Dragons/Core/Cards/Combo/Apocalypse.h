#pragma once

#include "../Card.h"
#include "../Utility/LifeExchange.h"
#include "../Utility/FrostFire.h"

class Core;

namespace cards
{
    class Apocalypse : public Card
    {
        bool _resumeLifeExchange = false;
        bool _resumeToFrostFire = false;
        bool _resumeFrostFire = false;
        bool _resumeToCardDraw = false;
        bool _waitingForCardDraw = false;
        bool _waitingForCardDiscard = false;
        bool _resumeToCleanUp = false;

        const std::vector<CardId> _requiredCardIds
        {
            LifeExchange::CARD_ID(),
            FrostFire::CARD_ID()
        };
        cards::Card* _cardLifeExchange = nullptr;
        cards::Card* _cardFrostFire = nullptr;

    public:
        Apocalypse() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "apocalypse" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new Apocalypse()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Apocalypse"; }
        std::wstring GetCardDescription() const { return L"Life Exchange effect\nFrost Fire effect\nDraw 10 cards\nDiscard cards until you have 10 or less cards."; }
        std::wstring GetCardFlavorText() const { return L"With the power to kill dragons"; }
    };
}