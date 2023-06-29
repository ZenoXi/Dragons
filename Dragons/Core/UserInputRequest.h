#pragma once

namespace cards
{
    class Card;
}

#include "Cards/CardType.h"
#include "Cards/CardPosition.h"

#include <string>
#include <vector>
#include <memory>

enum class UserInputType
{
    RESUME, // Resume card play immediatelly, without any parameters
    WAIT,
    WAIT_FOR_CONFIRMATION,
    YES_OR_NO,
    CHOOSE_NUMBER,
    CHOOSE_OPTION,
    CHOOSE_CARD_FROM_HAND,
    CHOOSE_CARD_FROM_GRAVEYARD,
    CHOOSE_CARD_FROM_ACTIVE_CARDS,
    CHOOSE_CARD_FROM_DISPLAYED_CARDS,
    CHOOSE_CARD_FROM_SET,
    CHOOSE_DECK,
    PLAY_CARD,
    DRAW_CARD,
    DISCARD_CARD
};

struct UserInputParams {};

//struct 

struct UserInputRequest
{
    UserInputType inputType = UserInputType::RESUME;
    std::wstring inputPrompt = L"";
    std::unique_ptr<UserInputParams> inputParams = nullptr;
};

struct UserInputResponse
{
    std::unique_ptr<UserInputParams> inputParams = nullptr;
};

struct UserInputParams_Wait : public UserInputParams
{
    int milliseconds;
};

struct UserInputParams_WaitForConfirmation : public UserInputParams
{
    // Request
    int playerIndex;

    // Response
};

struct UserInputParams_YesOrNo : public UserInputParams
{
    // Request
    int playerIndex;
    std::wstring yesText = L"Yes";
    std::wstring noText = L"No";

    // Response
    bool yes;
};

struct UserInputParams_ChooseNumber : public UserInputParams
{
    // Request
    int lowerBound;
    int upperBound;

    // Response
    int chosenNumber;
};

struct UserInputParams_ChooseOption : public UserInputParams
{
    // Request
    int playerIndex;
    std::vector<std::wstring> options;

    // Response
    int chosenOptionIndex;
};

struct UserInputParams_ChooseCardFromHand : public UserInputParams
{
    // Request
    int choosingPlayerIndex;
    int handPlayerIndex;
    int minCardCount;
    int maxCardCount;
    std::vector<cards::CardType> allowedTypes; // Empty list means no restriction

    // Response
    std::vector<cards::Card*> chosenCards;
};

struct UserInputParams_ChooseCardFromGraveyard : public UserInputParams
{
    // Request
    int playerIndex;
    int minCardCount;
    int maxCardCount;
    std::vector<cards::CardType> allowedTypes; // Empty list means no restriction

    // Response
    std::vector<cards::Card*> chosenCards;
};

struct UserInputParams_ChooseCardFromActiveCards : public UserInputParams
{
    // Request
    int choosingPlayerIndex;
    int targetActivesPlayerIndex;
    int minCardCount;
    int maxCardCount;
    std::vector<cards::CardType> allowedTypes; // Empty list means no restriction

    // Response
    std::vector<cards::Card*> chosenCards;
};

struct UserInputParams_ChooseCardFromDisplayedCards : public UserInputParams
{
    // Request
    int playerIndex;
    int minCardCount;
    int maxCardCount;
    std::vector<cards::CardType> allowedTypes; // Empty list means no restriction

    // Response
    std::vector<cards::Card*> chosenCards;
};

struct UserInputParams_ChooseCardFromSet : public UserInputParams
{
    // Request
    int playerIndex;
    int minCardCount;
    int maxCardCount;
    std::vector<cards::CardSet> allowedSets;
    std::vector<cards::CardType> allowedTypes; // Empty list means no restriction

    // Response
    std::vector<cards::Card*> chosenCards;
    std::vector<cards::CardSet> chosenCardSet;
};

struct UserInputParams_ChooseDeck : public UserInputParams
{
    // Request
    int playerIndex;
    int minDeckCount;
    int maxDeckCount;
    std::vector<cards::CardType> allowedDecks; // Empty list means no restriction
    bool allowEmptyDecks;

    // Response
    std::vector<cards::CardType> chosenDecks;
};

struct UserInputParams_PlayCard : public UserInputParams
{
    // Request
    int playerIndex;
    int minCardCount;
    int maxCardCount;
    std::vector<cards::CardType> allowedTypes; // Empty list means no restriction

    // Response
    std::vector<cards::Card*> playedCards;
};

struct UserInputParams_DrawCard : public UserInputParams
{
    // Request
    int playerIndex;
    int minCardCount;
    int maxCardCount;
    std::vector<cards::CardType> allowedTypes; // Empty list means no restriction
    bool ignoreHandSize = false;

    // Response
    std::vector<cards::Card*> drawnCards;
};

struct UserInputParams_DiscardCard : public UserInputParams
{
    // Request
    int playerIndex;
    int minCardCount;
    int maxCardCount;
    std::vector<cards::CardType> allowedTypes; // Empty list means no restriction

    // Response
    std::vector<cards::Card*> discardedCards;
};