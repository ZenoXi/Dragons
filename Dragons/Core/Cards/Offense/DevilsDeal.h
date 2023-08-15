#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class DevilsDeal : public Card
    {
        bool _waitingForCardChoice = false;
        Card* _chosenCard = nullptr;
        bool _resumeToCardPlay = false;
        bool _resumeCardPlay = false;

    public:
        DevilsDeal() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "devils_deal" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new DevilsDeal()); }

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Devils Deal"; }
        std::wstring GetCardDescription() const { return L"Force enemy to play one of his offensive cards on himself.\nIf enemy has no offensive cards he has to draw and play one on himself."; }
        std::wstring GetCardFlavorText() const { return L"Asking for devils help can cost a lot"; }
    };
}