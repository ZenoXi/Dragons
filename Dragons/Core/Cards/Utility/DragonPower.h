#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class DragonPower : public Card
    {
        bool _waitingForCardDraw = false;

    public:
        DragonPower() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "dragon_power" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new DragonPower()); }

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Dragon Power"; }
        std::wstring GetCardDescription() const { return L"If a deck is empty:\nOffensive - Deal 1 damage\nDefensive - Heal 1 health\nUtility - Draw a card\nCombo - All effects"; }
        std::wstring GetCardFlavorText() const { return L"3 most powerful dragons and one controlling them all"; }
    };
}