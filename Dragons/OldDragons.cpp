#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#include <set>

#include "Core/Core.h"
#include "Core/GameConstants.h"

struct PrintParams
{
    ActionProperties actionProps;
};

void PrintState(Core& core, PrintParams params = {})
{
    std::wostringstream ss;
    ss << " OffDeck (" << core.GetState().offenseDeck.size() << ")" << (core.CanDrawCard(cards::CardType::OFFENSE, core.GetState().currentPlayer) ? " " : " DISABLED")
       << " | DefDeck (" << core.GetState().defenseDeck.size() << ")" << (core.CanDrawCard(cards::CardType::DEFENSE, core.GetState().currentPlayer) ? " " : " DISABLED")
       << " | UtiDeck (" << core.GetState().utilityDeck.size() << ")" << (core.CanDrawCard(cards::CardType::UTILITY, core.GetState().currentPlayer) ? " " : " DISABLED")
       << " | ComDeck (" << core.GetState().comboDeck.size() << ")" << (core.CanDrawCard(cards::CardType::COMBO, core.GetState().currentPlayer) ? " " : " DISABLED") << '\n';

    ss << " Graveyard (" << core.GetState().graveyard.size() << ")\n";

    ss << std::fixed << std::setw(83) << std::setfill(L'-') << "" << std::setfill(L' ') << '\n';

    ss << std::setw(15) << std::fixed << "" << "| ";
    ss << std::fixed << std::setw(32) << std::left << "Player 1:" << "| ";
    ss << std::fixed << std::setw(32) << std::left << "Player 2:" << '\n';
    // Health
    ss << std::setw(15) << std::fixed << std::right << "Health " << "| ";
    ss << std::fixed << std::setw(32) << std::left << core.GetState().players[0].health << "| ";
    ss << std::fixed << std::setw(32) << std::left << core.GetState().players[1].health << '\n';
    // Max health
    ss << std::setw(15) << std::fixed << std::right << "Max health " << "| ";
    ss << std::fixed << std::setw(32) << std::left << core.GetState().players[0].maxHealth << "| ";
    ss << std::fixed << std::setw(32) << std::left << core.GetState().players[1].maxHealth << '\n';
    // Armor
    ss << std::setw(15) << std::fixed << std::right << "Armor " << "| ";
    ss << std::fixed << std::setw(32) << std::left << core.GetState().players[0].armor << "| ";
    ss << std::fixed << std::setw(32) << std::left << core.GetState().players[1].armor << '\n';

    ss << std::setw(15) << std::fixed << "" << "| ";
    ss << std::fixed << std::setw(32) << std::left << " " << "| ";
    ss << std::fixed << std::setw(32) << std::left << " " << '\n';

    ss << std::setw(15) << std::fixed << "" << "| ";
    ss << std::fixed << std::setw(32) << std::left << "Active cards:" << "| ";
    ss << std::fixed << std::setw(32) << std::left << "Active cards:" << '\n';

    int p1ActCount = core.GetState().players[0].activeCards.size();
    int p2ActCount = core.GetState().players[1].activeCards.size();
    int activeLines = std::max(p1ActCount, p2ActCount);

    for (int i = 0; i < activeLines; i++)
    {
        ss << std::setw(14) << std::fixed << std::right << i << " " << "| ";
        ss << (i < p1ActCount ? "> " : "  ") << std::fixed << std::setw(30) << std::left << (i < p1ActCount ? core.GetState().players[0].activeCards[i]->GetCardName() : L"") << "| ";
        ss << (i < p2ActCount ? "> " : "  ") << std::fixed << std::setw(30) << std::left << (i < p2ActCount ? core.GetState().players[1].activeCards[i]->GetCardName() : L"") << '\n';
    }

    ss << std::setw(15) << std::fixed << "" << "| ";
    ss << std::fixed << std::setw(32) << std::left << "" << "| ";
    ss << std::fixed << std::setw(32) << std::left << "" << '\n';

    ss << std::setw(15) << std::fixed << "" << "| ";
    ss << std::fixed << std::setw(32) << std::left << "Hand:" << (core.GetState().players[0].revealSources.empty() ? "" : " (revealed)") << "| ";
    ss << std::fixed << std::setw(32) << std::left << "Hand:" << (core.GetState().players[1].revealSources.empty() ? "" : " (revealed)") << '\n';

    int p1CardCount = core.GetState().players[0].hand.size();
    int p2CardCount = core.GetState().players[1].hand.size();
    int handLines = std::max(p1CardCount, p2CardCount);

    for (int i = 0; i < handLines; i++)
    {
        ss << std::setw(14) << std::fixed << std::right << i << " " << "| ";
        ss << (i < p1CardCount ? (core.CanPlayCard(core.GetState().players[0].hand[i].get(), std::optional(params.actionProps), nullptr) ? "> " : "X ") : "  ")
           << std::fixed << std::setw(30) << std::left << (i < p1CardCount ? core.GetState().players[0].hand[i]->GetCardName() : L"") << "| ";
        ss << (i < p2CardCount ? (core.CanPlayCard(core.GetState().players[1].hand[i].get(), std::optional(params.actionProps), nullptr) ? "> " : "X ") : "  ")
           << std::fixed << std::setw(30) << std::left << (i < p2CardCount ? core.GetState().players[1].hand[i]->GetCardName() : L"") << '\n';
    }

    ss << std::setw(15) << std::fixed << "" << "| ";
    ss << std::fixed << std::setw(32) << std::left << "" << "| ";
    ss << std::fixed << std::setw(32) << std::left << "" << '\n';

    ss << std::fixed << std::setw(83) << std::setfill(L'-') << "" << std::setfill(L' ') << '\n';

    ss << " Displayed cards:\n";
    for (int i = 0; i < core.GetState().displayedCards.size(); i++)
        ss << "  " << i << ": " << core.GetState().displayedCards[i].card->GetCardName() << '\n';

    ss << std::fixed << std::setw(83) << std::setfill(L'-') << "" << std::setfill(L' ') << '\n';

    ss << " In play cards:\n";
    for (int i = 0; i < core.GetState().inPlayCards.size(); i++)
        ss << "  " << i << ": " << core.GetState().inPlayCards[i]->GetCardName() << '\n';

    ss << std::fixed << std::setw(83) << std::setfill(L'-') << "" << std::setfill(L' ') << '\n';

    int currentPlayer = core.GetState().currentPlayer;
    ss << " Current player: " << currentPlayer + 1 << '\n';
    ss << " Actions left: " << core.GetState().players[currentPlayer].actionsLeft << '\n';
    if (!core.GetState().players[currentPlayer].extraActions.empty())
    {
        ss << " Extra actions: " << core.GetState().players[currentPlayer].extraActions.size() << '\n';
        for (auto& action : core.GetState().players[currentPlayer].extraActions)
            ss << " > " << (action.play ? 'P' : '-') << " " << (action.draw ? 'D' : '-') << " " << (action.discard ? 'd' : '-') << "\n";
    }

    ss << std::fixed << std::setw(83) << std::setfill(L'-') << "" << std::setfill(L' ') << '\n';

    system("cls");
    std::wcout << ss.str();
}

void HandleInputRequest(cards::PlayResult& result, Core& core, cards::Card* card)
{
    switch (result.inputRequest.inputType)
    {
    case UserInputType::RESUME:
    {
        UserInputResponse response;
        response.inputParams = std::move(result.inputRequest.inputParams);
        auto resumeResult = core.ResumePlay(std::move(response));

        if (resumeResult.waitForInput)
        {
            HandleInputRequest(resumeResult, core, card);
        }

        break;
    }
    case UserInputType::WAIT_FOR_CONFIRMATION:
    {
        while (true)
        {
            PrintState(core);
            std::cout << " WAIT FOR CONFIRMATION\n";
            std::wcout << "  " << result.inputRequest.inputPrompt << "\n\n";
            std::cout << " Type 'c' to continue\n";
            std::cout << " > ";

            std::string input;
            std::cin >> input;

            if (input == "c" || input == "C")
                break;
        }

        UserInputResponse response;
        response.inputParams = std::move(result.inputRequest.inputParams);
        auto resumeResult = core.ResumePlay(std::move(response));

        if (resumeResult.waitForInput)
        {
            HandleInputRequest(resumeResult, core, card);
        }

        break;
    }
    case UserInputType::YES_OR_NO:
    {
        auto params = reinterpret_cast<UserInputParams_YesOrNo*>(result.inputRequest.inputParams.get());

        while (true)
        {
            PrintState(core);
            std::cout << " YES OR NO\n";
            std::wcout << "  " << result.inputRequest.inputPrompt << "\n\n";
            std::cout << " Type 'y' or 'n' to continue\n";
            std::cout << " > ";

            std::string input;
            std::cin >> input;

            if (input == "y" || input == "Y")
            {
                params->yes = true;
                break;
            }
            else if (input == "n" || input == "N")
            {
                params->yes = false;
                break;
            }
        }

        UserInputResponse response;
        response.inputParams = std::move(result.inputRequest.inputParams);
        auto resumeResult = core.ResumePlay(std::move(response));

        if (resumeResult.waitForInput)
        {
            HandleInputRequest(resumeResult, core, card);
        }

        break;
    }
    case UserInputType::CHOOSE_NUMBER:
    {
        auto params = reinterpret_cast<UserInputParams_ChooseNumber*>(result.inputRequest.inputParams.get());

        while (true)
        {
            PrintState(core);
            std::cout << " CHOOSE NUMBER (min: " << params->lowerBound << ", max: " << params->upperBound << ")\n";
            std::wcout << " " << result.inputRequest.inputPrompt << "\n\n";

            int number;
            std::cin >> number;

            if (number < params->lowerBound || number > params->upperBound)
                continue;
            params->chosenNumber = number;

            break;
        }

        UserInputResponse response;
        response.inputParams = std::move(result.inputRequest.inputParams);
        auto resumeResult = core.ResumePlay(std::move(response));

        if (resumeResult.waitForInput)
        {
            HandleInputRequest(resumeResult, core, card);
        }

        break;
    }
    case UserInputType::CHOOSE_OPTION:
    {
        auto params = reinterpret_cast<UserInputParams_ChooseOption*>(result.inputRequest.inputParams.get());

        while (true)
        {
            PrintState(core);
            std::cout << " CHOOSE OPTION\n";
            std::wcout << " " << result.inputRequest.inputPrompt << "\n\n";
            std::wcout << " Options:\n";
            for (int i = 0; i < params->options.size(); i++)
                std::wcout << " " << i << ": " << params->options[i] << '\n';
            std::cout << '\n';

            std::cout << " > ";
            int index;
            std::cin >> index;

            if (index < 0 || index >= params->options.size())
                continue;
            params->chosenOptionIndex = index;

            break;
        }

        UserInputResponse response;
        response.inputParams = std::move(result.inputRequest.inputParams);
        auto resumeResult = core.ResumePlay(std::move(response));

        if (resumeResult.waitForInput)
        {
            HandleInputRequest(resumeResult, core, card);
        }

        break;
    }
    case UserInputType::CHOOSE_CARD_FROM_HAND:
    {
        auto params = reinterpret_cast<UserInputParams_ChooseCardFromHand*>(result.inputRequest.inputParams.get());

        while (true)
        {
            PrintState(core);
            std::cout << " CHOOSE CARDS FROM HAND (min: " << params->minCardCount << ", max: " << params->maxCardCount << ")\n";
            std::cout << "  Choosing: player " << params->choosingPlayerIndex + 1 << " from player " << params->handPlayerIndex + 1 << '\n';
            std::cout << "  Allowed card types:\n";
            if (params->allowedTypes.empty())
            {
                std::cout << "    all\n";
            }
            else
            {
                for (auto type : params->allowedTypes)
                {
                    std::cout << "    " << cards::CardTypeToString(type) << '\n';
                }
            }
            std::wcout << " " << result.inputRequest.inputPrompt << "\n\n";

            int maxCards = std::min(params->maxCardCount, (int)core.GetState().players[params->handPlayerIndex].hand.size());
            int minCards = std::min(params->minCardCount, (int)core.GetState().players[params->handPlayerIndex].hand.size());

            std::set<int> chosenCards;
            while (chosenCards.size() < maxCards)
            {
                int index;
                std::cin >> index;

                if (index == -1)
                    break;

                chosenCards.insert(index);
            }

            if (chosenCards.size() < minCards || chosenCards.size() > maxCards)
                continue;

            params->chosenCards.clear();
            for (auto index : chosenCards)
            {
                params->chosenCards.push_back(core.GetState().players[params->handPlayerIndex].hand[index].get());
            }

            break;
        }

        UserInputResponse response;
        response.inputParams = std::move(result.inputRequest.inputParams);
        auto resumeResult = core.ResumePlay(std::move(response));

        if (resumeResult.waitForInput)
        {
            HandleInputRequest(resumeResult, core, card);
        }

        break;
    }

    case UserInputType::CHOOSE_CARD_FROM_ACTIVE_CARDS:
    {
        auto params = reinterpret_cast<UserInputParams_ChooseCardFromActiveCards*>(result.inputRequest.inputParams.get());

        while (true)
        {
            PrintState(core);
            std::cout << " CHOOSE CARDS FROM ACTIVE CARDS (min: " << params->minCardCount << ", max: " << params->maxCardCount << ")\n";
            std::cout << "  Choosing: player " << params->choosingPlayerIndex + 1 << " from player " << params->targetActivesPlayerIndex + 1 << '\n';
            std::cout << "  Allowed card types:\n";
            if (params->allowedTypes.empty())
            {
                std::cout << "    all\n";
            }
            else
            {
                for (auto type : params->allowedTypes)
                {
                    std::cout << "    " << cards::CardTypeToString(type) << '\n';
                }
            }
            std::wcout << " " << result.inputRequest.inputPrompt << "\n\n";

            int maxCards = std::min(params->maxCardCount, (int)core.GetState().players[params->targetActivesPlayerIndex].activeCards.size());
            int minCards = std::min(params->minCardCount, (int)core.GetState().players[params->targetActivesPlayerIndex].activeCards.size());

            std::set<int> chosenCards;
            while (chosenCards.size() < maxCards)
            {
                int index;
                std::cin >> index;

                if (index == -1)
                    break;

                chosenCards.insert(index);
            }

            if (chosenCards.size() < minCards || chosenCards.size() > maxCards)
                continue;

            params->chosenCards.clear();
            for (auto index : chosenCards)
            {
                params->chosenCards.push_back(core.GetState().players[params->targetActivesPlayerIndex].activeCards[index].get());
            }

            break;
        }

        UserInputResponse response;
        response.inputParams = std::move(result.inputRequest.inputParams);
        auto resumeResult = core.ResumePlay(std::move(response));

        if (resumeResult.waitForInput)
        {
            HandleInputRequest(resumeResult, core, card);
        }

        break;
    }
    case UserInputType::CHOOSE_CARD_FROM_DISPLAYED_CARDS:
    {
        auto params = reinterpret_cast<UserInputParams_ChooseCardFromDisplayedCards*>(result.inputRequest.inputParams.get());

        while (true)
        {
            PrintState(core);
            std::cout << " CHOOSE CARDS FROM DISPLAYED CARDS (min: " << params->minCardCount << ", max: " << params->maxCardCount << ")\n";
            std::cout << "  Choosing: player " << params->playerIndex + 1 << '\n';
            std::cout << "  Allowed card types:\n";
            if (params->allowedTypes.empty())
            {
                std::cout << "    all\n";
            }
            else
            {
                for (auto type : params->allowedTypes)
                {
                    std::cout << "    " << cards::CardTypeToString(type) << '\n';
                }
            }
            std::wcout << " " << result.inputRequest.inputPrompt << "\n\n";

            std::set<int> chosenCards;
            while (chosenCards.size() < params->maxCardCount)
            {
                int index;
                std::cin >> index;

                if (index == -1)
                    break;

                chosenCards.insert(index);
            }

            if (chosenCards.size() < params->minCardCount || chosenCards.size() > params->maxCardCount)
                continue;

            params->chosenCards.clear();
            for (auto index : chosenCards)
            {
                params->chosenCards.push_back(core.GetState().displayedCards[index].card);
            }

            break;
        }

        UserInputResponse response;
        response.inputParams = std::move(result.inputRequest.inputParams);
        auto resumeResult = core.ResumePlay(std::move(response));

        if (resumeResult.waitForInput)
        {
            HandleInputRequest(resumeResult, core, card);
        }

        break;
    }
    case UserInputType::CHOOSE_CARD_FROM_GRAVEYARD:
    {
        auto params = reinterpret_cast<UserInputParams_ChooseCardFromGraveyard*>(result.inputRequest.inputParams.get());

        while (true)
        {
            system("cls");
            for (int i = 0; i < core.GetState().graveyard.size(); i++)
                std::wcout << i << ": " << core.GetState().graveyard[i]->GetCardName() << "\n";

            std::cout << " CHOOSE CARDS FROM GRAVEYARD (min: " << params->minCardCount << ", max: " << params->maxCardCount << ")\n";
            std::cout << "  Choosing: player " << params->playerIndex + 1 << '\n';
            std::cout << "  Allowed card types:\n";
            if (params->allowedTypes.empty())
            {
                std::cout << "    all\n";
            }
            else
            {
                for (auto type : params->allowedTypes)
                {
                    std::cout << "    " << cards::CardTypeToString(type) << '\n';
                }
            }
            std::wcout << " " << result.inputRequest.inputPrompt << "\n\n";

            std::set<int> chosenCards;
            while (chosenCards.size() < params->maxCardCount)
            {
                int index;
                std::cin >> index;

                if (index == -1)
                    break;

                chosenCards.insert(index);
            }

            if (chosenCards.size() < params->minCardCount || chosenCards.size() > params->maxCardCount)
                continue;

            params->chosenCards.clear();
            for (auto index : chosenCards)
            {
                params->chosenCards.push_back(core.GetState().graveyard[index].get());
            }

            break;
        }

        UserInputResponse response;
        response.inputParams = std::move(result.inputRequest.inputParams);
        auto resumeResult = core.ResumePlay(std::move(response));

        if (resumeResult.waitForInput)
        {
            HandleInputRequest(resumeResult, core, card);
        }

        break;
    }
    case UserInputType::CHOOSE_DECK:
    {
        auto params = reinterpret_cast<UserInputParams_ChooseDeck*>(result.inputRequest.inputParams.get());

        while (true)
        {
            PrintState(core);
            std::cout << " CHOOSE DECK (min: " << params->minDeckCount << ", max: " << params->maxDeckCount << ")\n";
            std::cout << "  Choosing: player " << params->playerIndex + 1 << '\n';
            std::cout << "  Empty decks: ";
            if (params->allowEmptyDecks)
                std::cout << "ALLOWED\n";
            else
                std::cout << "NOT ALLOWED\n";
            std::cout << "  Allowed deck types:\n";
            if (params->allowedDecks.empty())
            {
                std::cout << "    all\n";
            }
            else
            {
                for (auto type : params->allowedDecks)
                {
                    std::cout << "    " << cards::CardTypeToString(type) << '\n';
                }
            }
            std::wcout << " " << result.inputRequest.inputPrompt << "\n\n";

            std::set<cards::CardType> chosenDecks;
            while (chosenDecks.size() < params->maxDeckCount)
            {
                std::string deck;
                cards::CardType type;

                std::cout << " > ";
                std::cin >> deck;
                type = cards::CardType::NONE;
                if ((deck == "o" || deck == "off") && (!core.GetState().offenseDeck.empty() || params->allowEmptyDecks))
                    type = cards::CardType::OFFENSE;
                else if ((deck == "d" || deck == "def") && (!core.GetState().defenseDeck.empty() || params->allowEmptyDecks))
                    type = cards::CardType::DEFENSE;
                else if ((deck == "u" || deck == "uti") && (!core.GetState().utilityDeck.empty() || params->allowEmptyDecks))
                    type = cards::CardType::UTILITY;
                else if ((deck == "c" || deck == "com") && (!core.GetState().comboDeck.empty() || params->allowEmptyDecks))
                    type = cards::CardType::COMBO;
                else if (deck == "q" || deck == "quit")
                    break;
                if (type == cards::CardType::NONE)
                    continue;

                chosenDecks.insert(type);
            }

            if (chosenDecks.size() < params->minDeckCount || chosenDecks.size() > params->maxDeckCount)
                continue;

            params->chosenDecks.clear();
            for (auto& deck : chosenDecks)
                params->chosenDecks.push_back(deck);

            break;
        }

        UserInputResponse response;
        response.inputParams = std::move(result.inputRequest.inputParams);
        auto resumeResult = core.ResumePlay(std::move(response));

        if (resumeResult.waitForInput)
        {
            HandleInputRequest(resumeResult, core, card);
        }

        break;
    }
    case UserInputType::DRAW_CARD:
    {
        auto params = reinterpret_cast<UserInputParams_DrawCard*>(result.inputRequest.inputParams.get());

        while (true)
        {
            PrintState(core);
            std::cout << " DRAW CARD (min: " << params->minCardCount << ", max: " << params->maxCardCount << ")\n";
            if (params->ignoreHandSize)
                std::cout << "  HAND SIZE IGNORED\n";
            std::cout << "  Player: " << params->playerIndex + 1 << '\n';
            std::cout << "  Allowed decks:\n";
            if (params->allowedTypes.empty())
            {
                std::cout << "    all\n";
            }
            else
            {
                for (auto type : params->allowedTypes)
                {
                    std::cout << "    " << cards::CardTypeToString(type) << '\n';
                }
            }
            std::wcout << " " << result.inputRequest.inputPrompt << "\n";
            if (!params->drawnCards.empty())
            {
                std::cout << " Drawn cards:\n";
                for (auto card : params->drawnCards)
                {
                    std::wcout << "  " << card->GetCardName() << '\n';
                }
            }
            std::cout << '\n';

            if (params->drawnCards.size() >= params->maxCardCount)
                break;
            if (!params->ignoreHandSize && core.GetState().players[params->playerIndex].hand.size() >= GAME_HAND_SIZE)
                break;
            bool allDecksEmpty = core.GetState().offenseDeck.empty() && core.GetState().defenseDeck.empty() && core.GetState().utilityDeck.empty() && core.GetState().comboDeck.empty();
            if (allDecksEmpty)
                break;

            std::string cardType;
            cards::CardType type;

            std::cout << " > ";
            std::cin >> cardType;
            type = cards::CardType::NONE;
            if ((cardType == "o" || cardType == "off") && core.CanDrawCard(cards::CardType::OFFENSE, params->playerIndex) && !core.GetState().offenseDeck.empty())
                type = cards::CardType::OFFENSE;
            else if ((cardType == "d" || cardType == "def") && core.CanDrawCard(cards::CardType::DEFENSE, params->playerIndex) && !core.GetState().defenseDeck.empty())
                type = cards::CardType::DEFENSE;
            else if ((cardType == "u" || cardType == "uti") && core.CanDrawCard(cards::CardType::UTILITY, params->playerIndex) && !core.GetState().utilityDeck.empty())
                type = cards::CardType::UTILITY;
            else if ((cardType == "c" || cardType == "com") && core.CanDrawCard(cards::CardType::COMBO, params->playerIndex) && !core.GetState().comboDeck.empty())
                type = cards::CardType::COMBO;
            else if (cardType == "q" || cardType == "quit")
            {
                if (params->drawnCards.size() >= params->minCardCount)
                    break;
            }

            if (type == cards::CardType::NONE)
                continue;

            if (!params->allowedTypes.empty())
            {
                bool isAllowed = false;
                for (auto allowedType : params->allowedTypes)
                {
                    if (allowedType == type)
                    {
                        isAllowed = true;
                        break;
                    }
                }
                if (!isAllowed)
                    continue;
            }

            cards::Card* drawnCard = core.DrawCard(type, params->playerIndex, false);
            params->drawnCards.push_back(drawnCard);

            continue;
        }

        UserInputResponse response;
        response.inputParams = std::move(result.inputRequest.inputParams);
        auto resumeResult = core.ResumePlay(std::move(response));

        if (resumeResult.waitForInput)
        {
            HandleInputRequest(resumeResult, core, card);
        }

        break;
    }
    case UserInputType::PLAY_CARD:
    {
        auto params = reinterpret_cast<UserInputParams_PlayCard*>(result.inputRequest.inputParams.get());

        ActionProperties actionProps;
        actionProps.player = params->playerIndex;
        actionProps.opponent = params->opponentIndex;
        actionProps.freeAction = true;

        auto& playerHand = core.GetState().players[params->playerIndex].hand;

        while (true)
        {
            PrintState(core, { actionProps });
            std::cout << " PLAY CARD (min: " << params->minCardCount << ", max: " << params->maxCardCount << ")\n";
            std::cout << "  Player: " << params->playerIndex + 1 << " | Opponent: " << params->opponentIndex + 1 << '\n';
            std::cout << "  Allowed types:\n";
            if (params->allowedTypes.empty())
            {
                std::cout << "    all\n";
            }
            else
            {
                for (auto type : params->allowedTypes)
                {
                    std::cout << "    " << cards::CardTypeToString(type) << '\n';
                }
            }
            std::wcout << " " << result.inputRequest.inputPrompt << "\n";
            if (!params->playedCards.empty())
            {
                std::cout << " Played cards:\n";
                for (auto card : params->playedCards)
                {
                    std::wcout << "  " << card->GetCardName() << '\n';
                }
            }
            std::cout << '\n';

            if (params->playedCards.size() >= params->maxCardCount)
                break;
            if (core.GetState().players[params->playerIndex].CardsInHand(cards::CardType::OFFENSE) == 0)
                break;
            bool existPlayableCards = false;
            for (auto& card : playerHand)
            {
                if (!params->allowedTypes.empty())
                {
                    bool isAllowed = false;
                    for (auto allowedType : params->allowedTypes)
                    {
                        if (allowedType == card->GetCardType())
                        {
                            isAllowed = true;
                            break;
                        }
                    }
                    if (!isAllowed)
                        continue;
                }

                if (card->CanPlay(&core, actionProps, nullptr))
                {
                    existPlayableCards = true;
                    break;
                }
            }
            if (!existPlayableCards)
                break;

            int cardIndex;
            cards::Card* card;

            std::cout << " > ";
            std::cin >> cardIndex;
            card = nullptr;

            if (cardIndex != -1)
            {
                if (cardIndex < 0 || cardIndex >= playerHand.size())
                    continue;
                card = playerHand[cardIndex].get();
                if (!card->CanPlay(&core, actionProps, nullptr))
                    continue;
            }
            else
            {
                if (params->playedCards.size() >= params->minCardCount)
                    break;
                else
                    continue;
            }

            if (!params->allowedTypes.empty())
            {
                bool isAllowed = false;
                for (auto allowedType : params->allowedTypes)
                {
                    if (allowedType == card->GetCardType())
                    {
                        isAllowed = true;
                        break;
                    }
                }
                if (!isAllowed)
                    continue;
            }

            cards::PlayResult result = core.PlayCard(card, std::optional(actionProps), nullptr);
            if (result.waitForInput)
            {
                HandleInputRequest(result, core, card);
            }
            params->playedCards.push_back(card);

            continue;
        }

        UserInputResponse response;
        response.inputParams = std::move(result.inputRequest.inputParams);
        auto resumeResult = core.ResumePlay(std::move(response));

        if (resumeResult.waitForInput)
        {
            HandleInputRequest(resumeResult, core, card);
        }

        break;
    }
    case UserInputType::DISCARD_CARD:
    {
        auto params = reinterpret_cast<UserInputParams_DiscardCard*>(result.inputRequest.inputParams.get());

        auto& playerHand = core.GetState().players[params->playerIndex].hand;

        while (true)
        {
            PrintState(core);
            std::cout << " DISCARD CARD (min: " << params->minCardCount << ", max: " << params->maxCardCount << ")\n";
            std::cout << "  Player: " << params->playerIndex + 1 << '\n';
            std::cout << "  Allowed types:\n";
            if (params->allowedTypes.empty())
            {
                std::cout << "    all\n";
            }
            else
            {
                for (auto type : params->allowedTypes)
                {
                    std::cout << "    " << cards::CardTypeToString(type) << '\n';
                }
            }
            std::wcout << " " << result.inputRequest.inputPrompt << "\n";
            if (!params->discardedCards.empty())
            {
                std::cout << " Discarded cards:\n";
                for (auto card : params->discardedCards)
                {
                    std::wcout << "  " << card->GetCardName() << '\n';
                }
            }
            std::cout << '\n';

            if (params->discardedCards.size() >= params->maxCardCount)
                break;
            bool existDiscardableCards = false;
            for (auto& card : playerHand)
            {
                if (!params->allowedTypes.empty())
                {
                    bool isAllowed = false;
                    for (auto allowedType : params->allowedTypes)
                    {
                        if (allowedType == card->GetCardType())
                        {
                            isAllowed = true;
                            break;
                        }
                    }
                    if (!isAllowed)
                        continue;
                }

                existDiscardableCards = true;
                break;
            }
            if (!existDiscardableCards)
                break;

            int cardIndex;
            cards::Card* card;

            std::cout << " > ";
            std::cin >> cardIndex;
            card = nullptr;

            if (cardIndex != -1)
            {
                if (cardIndex < 0 || cardIndex >= playerHand.size())
                    continue;
                card = playerHand[cardIndex].get();
            }
            else
            {
                if (params->discardedCards.size() >= params->minCardCount)
                    break;
                else
                    continue;
            }

            if (!params->allowedTypes.empty())
            {
                bool isAllowed = false;
                for (auto allowedType : params->allowedTypes)
                {
                    if (allowedType == card->GetCardType())
                    {
                        isAllowed = true;
                        break;
                    }
                }
                if (!isAllowed)
                    continue;
            }

            cards::Card* discardedCard = core.DiscardCard(card, params->playerIndex, false);
            params->discardedCards.push_back(discardedCard);

            continue;
        }

        UserInputResponse response;
        response.inputParams = std::move(result.inputRequest.inputParams);
        auto resumeResult = core.ResumePlay(std::move(response));

        if (resumeResult.waitForInput)
        {
            HandleInputRequest(resumeResult, core, card);
        }

        break;
    }
    default:
    {
        std::wcout << "INPUT TYPE NOT IMPLEMENTED FOR CARD: " << card->GetCardName() << "\n";
        int k;
        std::cin >> k;
    }
    }
}

void PlayCard(Core& core, cards::Card* card)
{
    cards::PlayResult result = core.PlayCard(card);
    if (result.waitForInput)
    {
        HandleInputRequest(result, core, card);
    }
}

int oldmain()
{
    std::ios_base::sync_with_stdio(false);

    Core core;
    core.InitState();
    auto cardList = core.GetRegisteredCards();

    while (true)
    {
        PrintState(core);

        auto& currentPlayer = core.GetState().players[core.GetState().currentPlayer];

        std::cout << " > ";
        std::string input;
        std::cin >> input;
        
        if (input == "p" || input == "play")
        {
            int cardIndex;
            std::cin >> cardIndex;

            if (cardIndex >= currentPlayer.hand.size() || cardIndex < 0)
            {
                continue;
            }

            PlayCard(core, currentPlayer.hand[cardIndex].get());
        }
        else if (input == "d" || input == "draw")
        {
            std::string cardType;
            std::cin >> cardType;
            cards::CardType type = cards::CardType::NONE;
            if (cardType == "o" || cardType == "off")
                type = cards::CardType::OFFENSE;
            else if (cardType == "d" || cardType == "def")
                type = cards::CardType::DEFENSE;
            else if (cardType == "u" || cardType == "uti")
                type = cards::CardType::UTILITY;
            else if (cardType == "c" || cardType == "com")
                type = cards::CardType::COMBO;

            if (type == cards::CardType::NONE)
                continue;

            core.DrawCard(type, core.GetState().currentPlayer);
            continue;
        }
        else if (input == "dc" || input == "discard")
        {
            int cardIndex;
            std::cin >> cardIndex;

            if (cardIndex >= currentPlayer.hand.size() || cardIndex < 0)
            {
                continue;
            }

            core.DiscardCard(currentPlayer.hand[cardIndex].get(), core.GetState().currentPlayer);
            continue;
        }
        else if (input == "g" || input == "get")
        {
            std::string cardType;
            std::cin >> cardType;
            cards::CardType type = cards::CardType::NONE;
            if (cardType == "o" || cardType == "off")
                type = cards::CardType::OFFENSE;
            else if (cardType == "d" || cardType == "def")
                type = cards::CardType::DEFENSE;
            else if (cardType == "u" || cardType == "uti")
                type = cards::CardType::UTILITY;
            else if (cardType == "c" || cardType == "com")
                type = cards::CardType::COMBO;

            if (type == cards::CardType::NONE)
                continue;

            system("cls");

            for (int i = 0; i < cardList.size(); i++)
                if (cardList[i]->GetCardType() == type)
                    std::wcout << i << ": " << cardList[i]->GetCardName() << "\n";

            int cardIndex;
            std::cin >> cardIndex;

            if (cardIndex >= cardList.size() || cardIndex < 0)
            {
                continue;
            }

            core.AddCardToHand(cardList[cardIndex]->CreateInstance(), currentPlayer.index);
            continue;
        }
        else if (input == "gy" || input == "grave")
        {
            system("cls");
            for (int i = 0; i < core.GetState().graveyard.size(); i++)
                std::wcout << i << ": " << core.GetState().graveyard[i]->GetCardName() << "\n";

            int cardIndex;
            std::cin >> cardIndex;

            continue;
        }
        else if (input == "e" || input == "end")
        {
            core.EndTurn();
            continue;
        }
    }

    int n;
    std::cin >> n;

    int totalCount = 0;
    std::cout << "Starting path: " << std::filesystem::current_path() << '\n';
    for (auto& item : std::filesystem::recursive_directory_iterator(std::filesystem::current_path()))
    {
        if (!item.is_regular_file())
            continue;
        if (item.path().extension() != ".cpp" && item.path().extension() != ".h")
            continue;

        std::ifstream in(item.path());
        std::string line;
        int linesInFile = 0;
        while (std::getline(in, line))
        {
            linesInFile++;
        }
        in.close();

        std::cout << "Lines: " << linesInFile << " (" << item.path().string().substr(std::filesystem::current_path().string().length() + 1) << ")\n";
        totalCount += linesInFile;
    }
    std::cout << "TOTAL: " << totalCount << "\n";
}
