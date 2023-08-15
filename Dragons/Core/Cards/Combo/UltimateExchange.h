#pragma once

#include "../Card.h"
#include "../Utility/MindSteal.h"
#include "../Utility/DragonEquation.h"

class Core;

namespace cards
{
    class UltimateExchange : public Card
    {
        bool _resumeMindSteal = false;
        bool _resumeToDragonEquation = false;
        bool _waitingForHealthSwapChoice = false;
        bool _resumeToCleanUp = false;

        const std::vector<CardId> _requiredCardIds
        {
            MindSteal::CARD_ID(),
            DragonEquation::CARD_ID()
        };
        cards::Card* _cardMindSteal = nullptr;
        cards::Card* _cardDragonEquation = nullptr;

    public:
        UltimateExchange() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "ultimate_exchange" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new UltimateExchange()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Ultimate Exchange"; }
        std::wstring GetCardDescription() const { return L"Mind Swap effect\nDragon Equation effect\nYou can also swap health with your opponent."; }
        std::wstring GetCardFlavorText() const { return L"You don't even know if it's you anymore"; }
    };
}