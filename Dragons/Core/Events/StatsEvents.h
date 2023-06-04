#pragma once

#include "../DamageProperties.h"

struct PreDamageEvent_BuffPass
{
    static const char* _NAME_() { return "pre_damage_buff"; }
    DamageProperties* props;
};

struct PreDamageEvent_NerfPass
{
    static const char* _NAME_() { return "pre_damage_nerf"; }
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

struct PreHealthChangeEvent
{
    static const char* _NAME_() { return "pre_health_change"; }
    int target;
    int oldValue;
    int* newValue;
};

struct PostHealthChangeEvent
{
    static const char* _NAME_() { return "post_health_change"; }
    int target;
    int oldValue;
    int newValue;
};

struct PreArmorChangeEvent
{
    static const char* _NAME_() { return "pre_armor_change"; }
    int target;
    int oldValue;
    int* newValue;
};

struct PostArmorChangeEvent
{
    static const char* _NAME_() { return "post_armor_change"; }
    int target;
    int oldValue;
    int newValue;
};

struct PreMaxHealthChangeEvent
{
    static const char* _NAME_() { return "pre_max_health_change"; }
    int target;
    int oldValue;
    int* newValue;
};

struct PostMaxHealthChangeEvent
{
    static const char* _NAME_() { return "post_max_health_change"; }
    int target;
    int oldValue;
    int newValue;
};