#include "CardType.h"

std::string cards::CardTypeToString(CardType type)
{
    switch (type)
    {
    case CardType::OFFENSE:
        return "offense";
    case CardType::DEFENSE:
        return "defense";
    case CardType::UTILITY:
        return "utility";
    case CardType::COMBO:
        return "combo";
    default:
        return "unknown";
    }
}