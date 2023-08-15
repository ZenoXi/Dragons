#pragma once

#include "../Card.h"
#include "../Utility/Cheating.h"
#include "../Utility/TheThief.h"

class Core;

namespace cards
{
    class MagicTrick : public Card
    {
        bool _waitingForCardPick = false;
        bool _resumeToCardDraw = false;
        bool _waitingForCardDraw = false;
        bool _resumeToCleanUp = false;

        const std::vector<CardId> _requiredCardIds
        {
            Cheating::CARD_ID(),
            TheThief::CARD_ID()
        };
        cards::Card* _cardCheating = nullptr;
        cards::Card* _cardTheThief = nullptr;

    public:
        MagicTrick() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "magic_trick" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new MagicTrick()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Magic Trick"; }
        std::wstring GetCardDescription() const { return L"Cheating effect\nThe Thief effect\nYou can now choose a card from enemy hand. If opponent has two copies of your chosen card - take both, if he doesn't - draw a card"; }
        std::wstring GetCardFlavorText() const { return L"Rumor has it, that she stole the ankh staff herself"; }
    };
}