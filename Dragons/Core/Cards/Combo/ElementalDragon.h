#pragma once

#include "../Card.h"
#include "../Offense/HeavySlash.h"
#include "../Defense/ShieldingNet.h"
#include "../Utility/CorpsePuppet.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/ActionEvents.h"

class Core;

namespace cards
{
    class ElementalDragon : public Card
    {
        bool _waitingForResponse = false;
        bool _resumeToComboChoice = false;
        bool _waitingForComboChoice = false;
        Card* _chosenCard = nullptr;
        bool _resumeComboPlay = false;

        std::unique_ptr<EventHandler<PreGetComboCardsEvent>> _preGetComboCardsHandler = nullptr;

        const std::vector<CardId> _requiredCardIds
        {
            HeavySlash::CARD_ID(),
            ShieldingNet::CARD_ID(),
            CorpsePuppet::CARD_ID()
        };
        cards::Card* _cardHeavySlash = nullptr;
        cards::Card* _cardShieldingNet = nullptr;
        cards::Card* _cardCorpsePuppet = nullptr;

    public:
        ElementalDragon() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "elemental_dragon" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new ElementalDragon()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Elemental Dragon"; }
        std::wstring GetCardDescription() const { return L"Corpse Puppet effect\nHeavy Slash effect\nShielding Net effect\nYou can destroy all your active cards to activate another combo for free."; }
        std::wstring GetCardFlavorText() const { return L"It will help you for a price"; }
    };
}