#pragma once

#include "../Card.h"
#include "../Offense/DeathPoison.h"
#include "../Defense/HelpingHand.h"
#include "../Utility/SummonDead.h"

class Core;

namespace cards
{
    class DeathFee : public Card
    {
        bool _waitingToPickCardFromGraveyard = false;
        bool _waitingToSelectCard = false;

        const std::vector<CardId> _requiredCardIds
        {
            DeathPoison::CARD_ID(),
            HelpingHand::CARD_ID(),
            SummonDead::CARD_ID()
        };

    public:
        DeathFee() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "death_fee" }; }
        CardId GetCardId() const { return CARD_ID(); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Death Fee"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}