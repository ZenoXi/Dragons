#include "ResourceManager.h"

#include "StringHelper.h"

#include <wincodec.h>
#include <fstream>
#include <unordered_map>

std::vector<ImageResource> ResourceManager::_images;

std::unordered_map<std::string, std::string> resourceNameMap = {
    { "00", "blood_donation" },
    { "01", "death_poison" },
    { "02", "desperate_strike" },
    { "03", "devils_deal" },
    { "04", "fair_fight" },
    { "05", "fire_moon" },
    { "06", "heavy_slash" },
    { "07", "hellfire_sword" },
    { "08", "last_breath" },
    { "09", "rapid_attack" },
    { "10", "sacrificial_attack" },
    { "11", "stab" },
    { "12", "star_curse" },
    { "13", "vital_spot" },
    { "14", "weapon_scroll" },
    { "15", "armor_up" },
    { "16", "barrier" },
    { "17", "cure" },
    { "18", "divine_protection" },
    { "19", "healer" },
    { "20", "helping_hand" },
    { "21", "life_flower" },
    { "22", "peace_pact" },
    { "23", "preparations" },
    { "24", "recreation" },
    { "25", "shielding_net" },
    { "26", "soothing_spell" },
    { "27", "spirit_shelter" },
    { "28", "tree_of_life" },
    { "29", "war_blessing" },
    { "30", "absolute_zero" },
    { "31", "cheating" },
    { "32", "corpse_puppet" },
    { "33", "dragon_equation" },
    { "34", "dragon_flame" },
    { "35", "dragon_power" },
    { "36", "dragon_sight" },
    { "37", "frost_fire" },
    { "38", "hidden_treasures" },
    { "39", "life_exchange" },
    { "40", "mind_steal" },
    { "41", "set_up" },
    { "42", "summon_dead" },
    { "43", "the_thief" },
    { "44", "weapon_of_choice" },
    { "45", "apocalypse" },
    { "46", "astro_influence" },
    { "47", "counter_scale" },
    { "48", "death_fee" },
    { "49", "different_sides" },
    { "50", "double_strike" },
    { "51", "dragon_sword" },
    { "52", "dragon_weapons" },
    { "53", "dream_land" },
    { "54", "elemental_dragon" },
    { "55", "fake_out" },
    { "56", "final_hour" },
    { "57", "inferno_lifeline" },
    { "58", "magic_trick" },
    { "59", "nature_spell" },
    { "60", "omniclaustra" },
    { "61", "power_up" },
    { "62", "re_equip" },
    { "63", "ritual" },
    { "64", "shackling" },
    { "65", "shadow_assassin" },
    { "66", "soulstealer" },
    { "67", "superiority" },
    { "68", "swap_pact" },
    { "69", "three_gods_strike" },
    { "70", "total_entanglement" },
    { "71", "ultimate_defense" },
    { "72", "ultimate_exchange" },
    { "73", "underworld" },
    { "74", "vampirism" }
};

void ResourceManager::Init(std::string resourceFilePath, ID2D1DeviceContext* target)
{
    // Create 
    IWICImagingFactory* WICFactory;
    CoCreateInstance
    (
        CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&WICFactory)
    );

    std::ifstream in(resourceFilePath);
    while (true)
    {
        std::string resourceName;
        std::string resourcePath;
        in >> resourceName;
        in.get();
        bool eof = !std::getline(in, resourcePath);

        if (resourceNameMap.find(resourceName) != resourceNameMap.end())
            resourceName = resourceNameMap[resourceName];

        std::wstring resourcePathW = string_to_wstring(resourcePath);

        if (!eof)
        {
            HRESULT hr;

            IWICBitmapDecoder* decoder;
            hr = WICFactory->CreateDecoderFromFilename
            (
                resourcePathW.c_str(),
                NULL,
                GENERIC_READ,
                WICDecodeMetadataCacheOnLoad,
                &decoder
            );

            IWICBitmapFrameDecode* source;
            hr = decoder->GetFrame(0, &source);

            IWICFormatConverter* converter;
            hr = WICFactory->CreateFormatConverter(&converter);
            hr = converter->Initialize
            (
                source,
                GUID_WICPixelFormat32bppPBGRA,
                WICBitmapDitherTypeNone,
                NULL,
                0.f,
                WICBitmapPaletteTypeMedianCut
            );

            ID2D1Bitmap* bitmap;
            hr = target->CreateBitmapFromWicBitmap
            (
                converter,
                NULL,
                &bitmap
            );

            ResourceManager::_images.push_back({ resourceName, bitmap });

            decoder->Release();
            source->Release();
            converter->Release();
        }
        else
        {
            break;
        }
    }

    WICFactory->Release();
}

ID2D1Bitmap* ResourceManager::GetImage(std::string name)
{
    for (auto& resource : ResourceManager::_images)
    {
        if (resource.name == name)
        {
            return resource.bitmap;
        }
    }
    return nullptr;
}