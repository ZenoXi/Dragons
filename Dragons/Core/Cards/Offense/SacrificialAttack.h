#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    struct SacrificialAttackPlayProperties : public PlayProperties
    {
        bool damageSelf = true;
    };

    class SacrificialAttack : public Card
    {
    public:
        SacrificialAttack() {}

        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "sacrificial_attack" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new SacrificialAttack()); }

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Sacrificial Attack"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}