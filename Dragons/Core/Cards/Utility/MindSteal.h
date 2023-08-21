#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class MindSteal : public Card
    {
        bool _waitingForCardChoice = false;
        std::vector<Card*> _chosenCardsToGive;
        bool _waitingForOpponentCardChoice = false;

    public:
        MindSteal() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "mind_steal" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new MindSteal()); }

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Mind Steal"; }
        std::wstring GetCardDescription() const { return L"Swap any amount of cards with the enemy."; }
        std::wstring GetCardFlavorText() const { return L"Works best on smart players"; }
    };
}