#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    struct DragonSightPlayProperties : public PlayProperties
    {
        int cardRevealMultiplier = 1;
    };

    class DragonSight : public Card
    {
        bool _waitingForDeckChoice = false;
        bool _resumeToConfirmation = false;
        bool _waitingForConfirmation = false;

        int _decksRevealed;
        std::vector<std::vector<Card*>> _deckCopies;

    public:
        DragonSight() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "dragon_sight" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new DragonSight()); }

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Dragon Sight"; }
        std::wstring GetCardDescription() const { return L"Look at the top cards of the decks.\nIf one or more decks are empty, you can check 2 or more top cards of the decks up to 4 cards."; }
        std::wstring GetCardFlavorText() const { return L"One could sacrifice anything to gain sight of a dragon"; }
    };
}