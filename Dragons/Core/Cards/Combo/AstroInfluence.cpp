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
            _cardFairFight = card;
        else if (card->GetCardId() == TreeOfLife::CARD_ID())
            _cardTreeOfLife = card;
        else if (card->GetCardId() == DragonEquation::CARD_ID())
            _cardDragonEquation = card;
    }

    // Play Dragon Equation
    _cardDragonEquation->Play(core, actionProps, nullptr);

    // Play Fair Fight
    cards::FairFightPlayProperties fairFightPlayProps;
    fairFightPlayProps.giveOpponentArmor = false;
    _cardFairFight->Play(core, actionProps, &fairFightPlayProps);
    core->AddCardToActiveCards(core->RemoveCardFromInPlayCards(_cardFairFight), actionProps.player);

    // Play Tree of Life
    cards::TreeOfLifePlayProperties treeOfLifePlayProperties;
    treeOfLifePlayProperties.healOpponent = false;
    _cardTreeOfLife->Play(core, actionProps, &treeOfLifePlayProperties);

    core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardTreeOfLife));
    core->AddCardToGraveyard(core->RemoveCardFromInPlayCards(_cardDragonEquation));
    return PlayResult::Default();
}