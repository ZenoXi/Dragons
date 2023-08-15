#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    struct WeaponOfChoicePlayProperties : public PlayProperties
    {
        CardType deck = CardType::COMBO;
    };

    class WeaponOfChoice : public Card
    {
        bool _waitingForCardChoice = false;

    public:
        WeaponOfChoice() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "weapon_of_choice" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new WeaponOfChoice()); }

        CardType GetCardType() const { return CardType::UTILITY; }
        std::wstring GetCardName() const { return L"Weapon of Choice"; }
        std::wstring GetCardDescription() const { return L"Look at the top thre combo cards, take one, discard the other two."; }
        std::wstring GetCardFlavorText() const { return L"\"Best\" is a matter of perspective"; }
    };
}