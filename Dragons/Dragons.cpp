#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>

#include "Core/Core.h"
#include "Core/GameConstants.h"

void PrintState(Core& core)
{
    std::wostringstream ss;
    ss << " OffDeck (" << core.GetState().offenseDeck.size() << ") | DefDeck (" << core.GetState().defenseDeck.size() << ") | UtiDeck (" << core.GetState().utilityDeck.size() << ") | ComDeck (" << core.GetState().comboDeck.size() << ")" << '\n';
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
        ss << std::setw(15) << std::fixed << "" << "| ";
        ss << "> " << std::fixed << std::setw(32) << std::left << (i < p1ActCount ? core.GetState().players[0].activeCards[i]->GetCardName() : L"") << "| ";
        ss << "> " << std::fixed << std::setw(32) << std::left << (i < p2ActCount ? core.GetState().players[1].activeCards[i]->GetCardName() : L"") << '\n';
    }

    ss << std::setw(15) << std::fixed << "" << "| ";
    ss << std::fixed << std::setw(32) << std::left << "" << "| ";
    ss << std::fixed << std::setw(32) << std::left << "" << '\n';

    ss << std::setw(15) << std::fixed << "" << "| ";
    ss << std::fixed << std::setw(32) << std::left << "Hand:" << "| ";
    ss << std::fixed << std::setw(32) << std::left << "Hand:" << '\n';

    int p1CardCount = core.GetState().players[0].hand.size();
    int p2CardCount = core.GetState().players[1].hand.size();
    int handLines = std::max(p1ActCount, p2ActCount);

    for (int i = 0; i < handLines; i++)
    {
        ss << std::setw(15) << std::fixed << "" << "| ";
        ss << "> " << std::fixed << std::setw(32) << std::left << (i < p1CardCount ? core.GetState().players[0].hand[i]->GetCardName() : L"") << "| ";
        ss << "> " << std::fixed << std::setw(32) << std::left << (i < p2CardCount ? core.GetState().players[1].hand[i]->GetCardName() : L"") << '\n';
    }

    ss << std::setw(15) << std::fixed << "" << "| ";
    ss << std::fixed << std::setw(32) << std::left << "" << "| ";
    ss << std::fixed << std::setw(32) << std::left << "" << '\n';

    ss << std::fixed << std::setw(83) << std::setfill(L'-') << "" << std::setfill(L' ') << '\n';

    ss << " Displayed cards:\n";

    ss << std::fixed << std::setw(83) << std::setfill(L'-') << "" << std::setfill(L' ') << '\n';

    int currentPlayer = core.GetState().currentPlayer;
    ss << " Current player: " << currentPlayer + 1 << '\n';
    ss << " Actions left: " << core.GetState().players[currentPlayer].actionsLeft << '\n';
    if (!core.GetState().players[currentPlayer].extraActions.empty())
    {
        ss << " Extra actions: " << core.GetState().players[currentPlayer].actionsLeft << '\n';
        for (auto& action : core.GetState().players[currentPlayer].extraActions)
            ss << " > " << (action.play ? 'P' : '-') << " " << (action.draw ? 'D' : '-') << " " << (action.discard ? 'd' : '-') << "\n";
    }

    ss << std::fixed << std::setw(83) << std::setfill(L'-') << "" << std::setfill(L' ') << '\n';

    ss << " > ";

    system("cls");
    std::wcout << ss.str();
}

int main()
{
    std::ios_base::sync_with_stdio(false);

    Core core;
    core.InitState();

    while (true)
    {
        PrintState(core);

        std::string input;
        std::cin >> input;
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
