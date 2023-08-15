#pragma once

#include "../Card.h"
#include "../Offense/StarCurse.h"
#include "../Defense/WarBlessing.h"
#include "../Utility/MindSteal.h"

class Core;

namespace cards
{
    class SwapPact : public Card
    {
        bool _resumeMindSteal = false;
        bool _resumeToSwapChoice = false;
        bool _waitingForSwapChoice = false;
        bool _resumeToStarCurse = false;
        bool _resumeToCleanUp = false;

        const std::vector<CardId> _requiredCardIds
        {
            StarCurse::CARD_ID(),
            WarBlessing::CARD_ID(),
            MindSteal::CARD_ID()
        };
        cards::Card* _cardStarCurse = nullptr;
        cards::Card* _cardWarBlessing = nullptr;
        cards::Card* _cardMindSteal = nullptr;

    public:
        SwapPact() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "swap_pact" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new SwapPact()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Swap Pact"; }
        std::wstring GetCardDescription() const { return L"Mind Steal effect\nStar Curse effect\nWar Blessing effect\nAfter swapping cards, you choose if you want to swap maximum health with your opponent before applying Star Curse and War Blessing."; }
        std::wstring GetCardFlavorText() const { return L"With signature from the god of death himself"; }
    };
}