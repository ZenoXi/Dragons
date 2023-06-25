#pragma once

#include "../Card.h"
#include "../Utility/SummonDead.h"
#include "../Utility/DragonFlame.h"

class Core;

namespace cards
{
    class Underworld : public Card
    {
        bool _resumeSummonDead = false;
        bool _resumeToDragonFlame = false;
        bool _waitingForCardChoice = false;
        bool _resumeToCleanUp = false;

        const std::vector<CardId> _requiredCardIds
        {
            SummonDead::CARD_ID(),
            DragonFlame::CARD_ID()
        };
        cards::Card* _cardSummonDead = nullptr;
        cards::Card* _cardDragonFlame = nullptr;

    public:
        Underworld() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "underworld" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new Underworld()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Underworld"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}