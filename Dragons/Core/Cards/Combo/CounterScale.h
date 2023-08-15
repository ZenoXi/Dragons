#pragma once

#include "../Card.h"
#include "../Offense/RapidAttack.h"
#include "../Defense/Preparations.h"
#include "../Utility/DragonPower.h"

class Core;

namespace cards
{
    class CounterScale : public Card
    {
        bool _resumeDragonPower = false;
        bool _resumeToRapidAttack = false;
        bool _resumeToCleanUp = false;

        const std::vector<CardId> _requiredCardIds
        {
            RapidAttack::CARD_ID(),
            Preparations::CARD_ID(),
            DragonPower::CARD_ID()
        };
        cards::Card* _cardRapidAttack = nullptr;
        cards::Card* _cardPreparations = nullptr;
        cards::Card* _cardDragonPower = nullptr;

    public:
        CounterScale() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "counter_scale" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new CounterScale()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Counter Scale"; }
        std::wstring GetCardDescription() const { return L"Dragon Power effect\nRapid Attack effect\nPreparations effect\nGain 1 armor for each offensive card your opponent has, deal 1 damage for each defensive card your opponent has, combo effect for each deck that is empty."; }
        std::wstring GetCardFlavorText() const { return L"To find the perfect balance"; }
    };
}