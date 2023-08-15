#pragma once

#include "../Card.h"
#include "../Offense/WeaponScroll.h"
#include "../Defense/PeacePact.h"
#include "../Utility/WeaponOfChoice.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/CardStateEvents.h"

class Core;

namespace cards
{
    class ReEquip : public Card
    {
        bool _resumeWeaponOfChoice = false;
        bool _resumeToWeaponScroll = false;
        bool _resumeWeaponScroll = false;
        bool _resumeToPeacePact = false;
        bool _resumeToCardChoice = false;
        bool _waitingForCardChoice = false;
        bool _resumeToCleanUp = false;

        std::unique_ptr<EventHandler<CardLeaveHandEvent>> _cardLeaveHandHandler = nullptr;
        std::unique_ptr<EventHandler<CardEnterGraveyardEvent>> _cardEnterGraveyardHandler = nullptr;
        std::unique_ptr<EventHandler<CardEnterDeckEvent>> _cardEnterDeckHandler = nullptr;
        bool _ignoreCard = false;
        std::vector<Card*> _discardedOffenseCards;

        const std::vector<CardId> _requiredCardIds
        {
            WeaponScroll::CARD_ID(),
            PeacePact::CARD_ID(),
            WeaponOfChoice::CARD_ID()
        };
        cards::Card* _cardWeaponScroll = nullptr;
        cards::Card* _cardPeacePact = nullptr;
        cards::Card* _cardWeaponOfChoice = nullptr;

    public:
        ReEquip() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "re_equip" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new ReEquip()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Re-Equip"; }
        std::wstring GetCardDescription() const { return L"Weapon of Choice effect\nWeapon Scroll effect\nPeace Pact effect\nYou can redraw every offensive card you discarded using Weapon of Choice effect."; }
        std::wstring GetCardFlavorText() const { return L"Some pacts are deemed to be broken"; }
    };
}