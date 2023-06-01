#pragma once

#include "../DamageProperties.h"

struct PreDamageEvent
{
    static const char* _NAME_() { return "pre_damage"; }
    DamageProperties* props;
};

struct PostDamageEvent
{
    static const char* _NAME_() { return "post_damage"; }
};

struct PreHealEvent
{
    static const char* _NAME_() { return "pre_heal"; }
    int* target;
    int* amount;
};

struct PostHealEvent
{
    static const char* _NAME_() { return "post_heal"; }
};

struct PreAddArmorEvent
{
    static const char* _NAME_() { return "pre_add_armor"; }
    int* target;
    int* amount;
};

struct PostAddArmorEvent
{
    static const char* _NAME_() { return "post_add_armor"; }
};

struct PreDestroyArmorEvent
{
    static const char* _NAME_() { return "pre_destroy_armor"; }
    int* target;
    bool* cancel;
};

struct PostDestroyArmorEvent
{
    static const char* _NAME_() { return "post_destroy_armor"; }
};

struct PreSetMaxHealthEvent
{
    static const char* _NAME_() { return "pre_set_max_health"; }
    int* target;
    int* value;
};

struct PostSetMaxHealthEvent
{
    static const char* _NAME_() { return "post_set_max_health"; }
};