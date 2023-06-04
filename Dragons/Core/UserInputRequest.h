#pragma once

namespace cards
{
    class Card;
}

#include "Cards/CardType.h"

#include <string>
#include <vector>
#include <memory>

enum class UserInputType
{
    WAIT,
    CHOOSE_NUMBER,
    WAIT_FOR_CONFIRMATION,
    CHOOSE_CARD_FROM_HAND,
    CHOOSE_CARD_FROM_GRAVEYARD,
    CHOOSE_CARD_FROM_ACTIVE_CARDS,
    CHOOSE_CARD_FROM_CENTER,
    CHOOSE_DECK,
    PLAY_CARD,
    DRAW_CARD,
    DISCARD_CARD
};

struct UserInputParams {};

//struct 

struct UserInputRequest
{
    UserInputType inputType = UserInputType::WAIT;
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

struct UserInputParams_ChooseNumber : public UserInputParams
{
    // Request
    int lowerBound;
    int upperBound;

    // Response
    int chosenNumber;
};

struct UserInputParams_WaitForConfirmation : public UserInputParams
{
    // Request

    // Response
};

struct UserInputParams_ChooseCardFromHand : public UserInputParams
{
    // Request
    int playerIndex;
    int minCardCount;
    int maxCardCount;
    std::vector<cards::CardType> allowedTypes; // Empty list means no restriction

    // Response
    std::vector<cards::Card*> chosenCards;
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

    // Response
    std::vector<cards::Card*> drawnCards;
};