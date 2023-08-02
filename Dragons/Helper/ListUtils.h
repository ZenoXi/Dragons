#pragma once

#include <algorithm>

namespace zutil
{
    template<class T, typename I>
    bool Contains(T list, I item)
    {
        return std::find(list.cbegin(), list.cend(), item) != list.cend();
    }
}