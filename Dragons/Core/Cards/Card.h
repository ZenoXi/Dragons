#pragma once

#include "CardId.h"
#include "CardType.h"
#include "CardPosition.h"
#include "../ActionProperties.h"
#include "../UserInputRequest.h"

#include <string>

class Core;

namespace cards
{
    // Used to pass customization data to played card
    struct PlayProperties {};

    struct PlayResult
    {
        bool discard = true;
        bool notPlayed = false;
        bool waitForInput = false;
        UserInputRequest inputRequest{};
        bool resume = false;

        static PlayResult Default()
        {
            return PlayResult{};
        }
        static PlayResult NotPlayed()
        {
            PlayResult result;
            result.notPlayed = true;
            return result;
        }
        static PlayResult DontDiscard()
        {
            PlayResult result;
            result.discard = false;
            return result;
        }
        static PlayResult Resume()
        {
            PlayResult result;
            result.resume = true;
            return result;
        }
    };

    class Card
    {
        CardPosition _position;

    public:
        virtual bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps) { return true; }
        virtual PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps) = 0;
        virtual PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps) { return PlayResult{}; }
        virtual void Draw(Core* core, ActionProperties actionProps) {}
        virtual void Discard(Core* core, ActionProperties actionProps) {}
        virtual bool IsActive() { return false; }
        virtual int GetActionCost() { return 1; }

        // Metadata
        virtual CardId GetCardId() const = 0;

        virtual CardType GetCardType() const = 0;
        virtual std::wstring GetCardName() const = 0;
        virtual std::wstring GetCardDescription() const = 0;

        // Card position
        CardPosition GetPosition() const { return _position; }
        void OnEnterHand(Core* core, int playerIndex)
        {
            _OnEnterHand(core, playerIndex);
            _position.set = CardPosition::Set::HAND;
            _position.playerIndex = playerIndex;
        }
        void OnEnterActiveCards(Core* core, int playerIndex)
        {
            _OnEnterActiveCards(core, playerIndex);
            _position.set = CardPosition::Set::ACTIVE_CARDS;
            _position.playerIndex = playerIndex;
        }
        void OnEnterDeck(Core* core)
        {
            _OnEnterDeck(core);
            _position.set = CardPosition::Set::DECK;
            _position.playerIndex = -1;
        }
        void OnEnterGraveyard(Core* core)
        {
            _OnEnterGraveyard(core);
            _position.set = CardPosition::Set::GRAVEYARD;
            _position.playerIndex = -1;
        }
        void OnEnterDestroyedCards(Core* core)
        {
            _OnEnterDestroyedCards(core);
            _position.set = CardPosition::Set::DESTROYED;
            _position.playerIndex = -1;
        }

    private:
        virtual void _OnEnterHand(Core* core, int playerIndex) {}
        virtual void _OnEnterActiveCards(Core* core, int playerIndex) {}
        virtual void _OnEnterDeck(Core* core) {}
        virtual void _OnEnterGraveyard(Core* core) {}
        virtual void _OnEnterDestroyedCards(Core* core) {}
    };
}
