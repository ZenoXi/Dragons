#pragma once

#include "../Card.h"
#include "../Offense/SacrificialAttack.h"
#include "../Defense/Barrier.h"
#include "../Utility/DragonSight.h"

class Core;

namespace cards
{
    class ThreeGodsStrike : public Card
    {
        bool _waitingForEffectChoice = false;
        bool _resumeToDragonSight = false;
        bool _resumeDragonSight = false;
        bool _resumeToSacrificialAttack = false;
        bool _resumeToBarrier = false;
        bool _resumeToCleanUp = false;

        bool _buffSacrificialAttack = false;
        bool _buffBarrier = false;
        bool _buffDragonSight = false;

        const std::vector<CardId> _requiredCardIds
        {
            SacrificialAttack::CARD_ID(),
            Barrier::CARD_ID(),
            DragonSight::CARD_ID()
        };
        cards::Card* _cardSacrificialAttack = nullptr;
        cards::Card* _cardBarrier = nullptr;
        cards::Card* _cardDragonSight = nullptr;

    public:
        ThreeGodsStrike() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "three_gods_strike" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new ThreeGodsStrike()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Three Gods Strike"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}