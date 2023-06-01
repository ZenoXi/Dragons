#pragma once

#include "../Card.h"

class Core;

namespace cards
{
    struct BloodDonationProperties : public PlayProperties
    {
        int damageAmount;
    };

    class BloodDonation : public Card
    {
    public:
        BloodDonation() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);

        CardType GetCardType() { return CardType::OFFENSE; }
        std::wstring GetCardName() { return L"Blood Donation"; }
        std::wstring GetCardDescription() { return L""; }
    };
}