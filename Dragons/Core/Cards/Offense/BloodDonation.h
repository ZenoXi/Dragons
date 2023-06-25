#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    class BloodDonation : public Card
    {
        bool _waitingForDamageInput = false;
        bool _waitingToDrawCards = false;

    public:
        BloodDonation() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "blood_donation" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new BloodDonation()); }

        CardType GetCardType() const { return CardType::OFFENSE; }
        std::wstring GetCardName() const { return L"Blood Donation"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}