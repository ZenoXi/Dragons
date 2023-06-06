#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

int main()
{
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
    int n;
    std::cin >> n;
}
