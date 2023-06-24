#include "AstroInfluence.h"

#include "../../Core.h"
#include "../../GameConstants.h"

bool cards::AstroInfluence::CanPlay(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    return core->CanPlayComboCard(ComboProperties{ _requiredCardIds, actionProps.player }, actionProps, playProps);
}

cards::PlayResult cards::AstroInfluence::Play(Core* core, ActionProperties actionProps, PlayProperties* playProps)
{
    auto cardsForCombo = core->GetCardsForCombo(ComboProperties{ _requiredCardIds, actionProps.player });
    for (auto& card : cardsForCombo)
    {
        if (card->GetCardId() == FairFight::CARD_ID())
            _cardFairFight = std::move(card);
        else if (card->GetCardId() == TreeOfLife::CARD_ID())
            _cardTreeOfLife = std::move(card);
        else if (card->GetCardId() == DragonEquation::CARD_ID())
            _cardDragonEquation = std::move(card);
    }
    cardsForCombo.clear();

    // Play Dragon Equation
    _cardDragonEquation->Play(core, actionProps, nullptr);

    // Play Fair Fight
    cards::FairFightPlayProperties fairFightPlayProps;
    fairFightPlayProps.giveOpponentArmor = false;
    _cardFairFight->Play(core, actionProps, &fairFightPlayProps);

    // Play Tree of Life
    cards::TreeOfLifePlayProperties treeOfLifePlayProperties;
    treeOfLifePlayProperties.healOpponent = false;
    _cardTreeOfLife->Play(core, actionProps, &treeOfLifePlayProperties);

    return PlayResult::Resume();

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: move fair fight to active cards and rework combo card retrieval to move cards to the in play set when playing combos
}