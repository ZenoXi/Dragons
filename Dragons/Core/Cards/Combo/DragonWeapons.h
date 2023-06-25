#pragma once

#include "../Card.h"
#include "../Utility/DragonSight.h"
#include "../Utility/WeaponOfChoice.h"

class Core;

namespace cards
{
    class DragonWeapons : public Card
    {
        bool _resumeDragonSight = false;
        bool _resumeToWeaponOfChoice = false;
        bool _waitingForDeckChoice = false;
        bool _resumeToWeaponOfChoiceEffect = false;
        bool _resumeWeaponOfChoiceEffect = false;
        bool _resumeToCleanUp = false;

        std::vector<CardType> _decksToView;

        const std::vector<CardId> _requiredCardIds
        {
            DragonSight::CARD_ID(),
            WeaponOfChoice::CARD_ID()
        };
        cards::Card* _cardDragonSight = nullptr;
        cards::Card* _cardWeaponOfChoice = nullptr;

    public:
        DragonWeapons() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "dragon_weapons" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new DragonWeapons()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Dragon Weapons"; }
        std::wstring GetCardDescription() const { return L""; }
    };
}