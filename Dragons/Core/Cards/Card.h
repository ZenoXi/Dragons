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

    template<class T>
    T GetPlayProperties(PlayProperties* playProps)
    {
        if (playProps)
        {
            T* playPropsCastPtr = reinterpret_cast<T*>(playProps);
            if (playPropsCastPtr)
            {
                return *playPropsCastPtr;
            }
        }
        return T{};
    }

    struct PlayResult
    {
        bool discard = true;
        bool addToActives = false;
        bool notPlayed = false;
        bool waitForInput = false;
        UserInputRequest inputRequest{};

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
        static PlayResult AddToActives()
        {
            PlayResult result;
            result.discard = false;
            result.addToActives = true;
            return result;
        }
        static PlayResult Resume()
        {
            PlayResult result;
            result.waitForInput = true;
            return result;
        }
    };

    class Card
    {
        CardSet _position;

        bool _virtual;
        std::vector<CardSet> _allowedSets;

    public:
        virtual bool CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps) { return true; }
        virtual PlayResult Play(Core* core, ActionProperties actionProps, PlayProperties* playProps) = 0;
        virtual PlayResult Resume(UserInputResponse response, Core* core, ActionProperties actionProps, PlayProperties* playProps) { return PlayResult{}; }
        virtual void Draw(Core* core, ActionProperties actionProps) {}
        virtual void Discard(Core* core, ActionProperties actionProps) {}
        virtual bool IsActive() { return false; }
        virtual int GetActionCost() { return 1; }

        // Virtual cards get erased when moved to non allowed sets
        bool IsVirtual() const { return _virtual; }
        void SetVirtual(bool value) { _virtual = value; }
        std::vector<CardSet> GetAllowedSets() const { return _allowedSets; }
        void SetAllowedSets(std::vector<CardSet> sets) { _allowedSets = sets; }
        bool IsAllowedSet(CardSet set) const
        {
            for (auto& allowedSet : _allowedSets)
            {
                if (allowedSet.set == set.set && allowedSet.playerIndex == set.playerIndex)
                {
                    return true;
                }
            }
            return false;
        }

        // Metadata
        virtual CardId GetCardId() const = 0;
        virtual std::unique_ptr<Card> CreateInstance() = 0;

        virtual CardType GetCardType() const = 0;
        virtual std::wstring GetCardName() const = 0;
        virtual std::wstring GetCardDescription() const = 0;

        // Card position
        CardSet GetPosition() const { return _position; }
        void OnEnterHand(Core* core, int playerIndex)
        {
            _OnEnterHand(core, playerIndex);
            _position.set = CardSets::HAND;
            _position.playerIndex = playerIndex;
        }
        void OnEnterActiveCards(Core* core, int playerIndex)
        {
            _OnEnterActiveCards(core, playerIndex);
            _position.set = CardSets::ACTIVE_CARDS;
            _position.playerIndex = playerIndex;
        }
        void OnEnterDeck(Core* core)
        {
            _OnEnterDeck(core);
            _position.set = CardSets::DECK;
            _position.playerIndex = -1;
        }
        void OnEnterGraveyard(Core* core)
        {
            _OnEnterGraveyard(core);
            _position.set = CardSets::GRAVEYARD;
            _position.playerIndex = -1;
        }
        void OnEnterDestroyedCards(Core* core)
        {
            _OnEnterDestroyedCards(core);
            _position.set = CardSets::DESTROYED;
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
