#pragma once

#include "../Card.h"
#include "../Offense/Stab.h"
#include "../Defense/Cure.h"
#include "../Utility/TheThief.h"
#include "../../Events/EventSubscriber.h"
#include "../../Events/StatsEvents.h"

class Core;

namespace cards
{
    class Soulstealer : public Card
    {
        bool _resumeTheThief = false;
        bool _resumeToHealthSteal = false;
        bool _resumeToCleanUp = false;

        std::unique_ptr<EventHandler<PostHealEvent>> _postHealHandler = nullptr;

        const std::vector<CardId> _requiredCardIds
        {
            Stab::CARD_ID(),
            Cure::CARD_ID(),
            TheThief::CARD_ID()
        };
        cards::Card* _cardStab = nullptr;
        cards::Card* _cardCure = nullptr;
        cards::Card* _cardTheThief = nullptr;

    public:
        Soulstealer() {}

        bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps);
        PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps);

        static CardId CARD_ID() { return { "soulstealer" }; }
        CardId GetCardId() const { return CARD_ID(); }
        std::unique_ptr<Card> CreateInstance() { return std::unique_ptr<Card>(new Soulstealer()); }

        CardType GetCardType() const { return CardType::COMBO; }
        std::wstring GetCardName() const { return L"Soulstealer"; }
        std::wstring GetCardDescription() const { return L"The Thief effect\nStab effect\nCure effect\nInstead of damaging and healing, you steal health equal to cure from opponent. (Damages through all defenses)"; }
        std::wstring GetCardFlavorText() const { return L"A stab from this sword will leave a wound in your soul"; }
    };
}