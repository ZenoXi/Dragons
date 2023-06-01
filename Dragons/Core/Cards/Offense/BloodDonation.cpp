#include "BloodDonation.h"

#include "../../Core.h"

bool cards::BloodDonation::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    if (playProps)
    {
        BloodDonationProperties props = *static_cast<BloodDonationProperties*>(playProps);

        // Check 
    }
    else
    {
        return true;
    }
}

cards::PlayResult cards::BloodDonation::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    DamageProperties damageProps;
    damageProps.source = actionProps.player;
    damageProps.target = actionProps.opponent;
    damageProps.amount = 3;
    core->Damage(damageProps);

    damageProps.source = -1;
    damageProps.target = actionProps.player;
    core->Damage(damageProps);

    return PlayResult{};
}