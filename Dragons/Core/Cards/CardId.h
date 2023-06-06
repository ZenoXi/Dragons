#pragma once

struct CardId
{
    const char* name = nullptr;

    bool operator==(const CardId& other)
    {
        return name != nullptr && name == other.name;
    }
};