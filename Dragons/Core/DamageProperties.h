#pragma once

struct DamageProperties
{
    int source = -1;
    int target = -1;
    int amount = 0;
    bool ignoreArmor = false;
    bool trueDamage = false;
};