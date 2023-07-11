#include "Board.h"
#include "App.h"

void zcom::Board::_OnUpdate()
{

}

void zcom::Board::_OnDraw(Graphics g)
{

}

void zcom::Board::_CreateMissingCards()
{
    for (auto& cardPtr : core->GetState().offenseDeck)
    {
        auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
        if (it == _cards.end())
            _cards.push_back(_Card{ cardPtr.get() });
    }
    for (auto& cardPtr : core->GetState().defenseDeck)
    {
        auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
        if (it == _cards.end())
            _cards.push_back(_Card{ cardPtr.get() });
    }
    for (auto& cardPtr : core->GetState().utilityDeck)
    {
        auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
        if (it == _cards.end())
            _cards.push_back(_Card{ cardPtr.get() });
    }
    for (auto& cardPtr : core->GetState().comboDeck)
    {
        auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
        if (it == _cards.end())
            _cards.push_back(_Card{ cardPtr.get() });
    }
    for (auto& player : core->GetState().players)
    {
        for (auto& cardPtr : player.hand)
        {
            auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
            if (it == _cards.end())
                _cards.push_back(_Card{ cardPtr.get() });
        }
        for (auto& cardPtr : player.activeCards)
        {
            auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
            if (it == _cards.end())
                _cards.push_back(_Card{ cardPtr.get() });
        }
    }
    for (auto& cardPtr : core->GetState().graveyard)
    {
        auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
        if (it == _cards.end())
            _cards.push_back(_Card{ cardPtr.get() });
    }
    for (auto& cardPtr : core->GetState().inPlayCards)
    {
        auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
        if (it == _cards.end())
            _cards.push_back(_Card{ cardPtr.get() });
    }
    for (auto& cardPtr : core->GetState().destroyedCards)
    {
        auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
        if (it == _cards.end())
            _cards.push_back(_Card{ cardPtr.get() });
    }
}

void zcom::Board::_CalculateCardTargetPositions()
{
    float viewWidth = GetWidth();
    float viewHeight = GetHeight();

    for (auto& card : _cards)
    {
        cards::CardSet set = core->GetCardSet(card.card);
        switch (set.set)
        {
        case cards::CardSets::DECK:
        {

        }
        default:
        {
            // 
            break;
        }
        }
    }
}