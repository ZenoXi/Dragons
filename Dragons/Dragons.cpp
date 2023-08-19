#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include "Window/WindowsEx.h"
// Necessary for clsid's to work when creating custom effects
#include <Mmsystem.h>
#include <shlobj.h>
#include <shobjidl_core.h>

#include "Scenes/ConnectionScene.h"
#include "Scenes/EntryScene.h"

#include "Helper/ResourceManager.h"
#include "Helper/Time.h"
#include "Window/DisplayWindow.h"
#include "App.h"

#pragma comment( lib,"Winmm.lib" )

int WINAPI main(HINSTANCE hInst, HINSTANCE, LPWSTR cmdLine, INT)
{
    // Set working directory
    std::wstring dir;
    dir.resize(MAX_PATH);
    GetModuleFileName(NULL, dir.data(), MAX_PATH);
    auto pos = dir.find_last_of(L"\\/");
    std::wstring runDir = dir.substr(0, pos);
    std::wcout << "Executable path: " << dir << '\n';

    if (!SetCurrentDirectory(runDir.data()))
    {
        std::cout << "Directory set failed\n";
        return -1;
    }

    TCHAR path[MAX_PATH] = { 0 };
    DWORD a = GetCurrentDirectory(MAX_PATH, path);
    std::wcout << "New working directory: " << path << '\n';

    // Read arguments
    std::vector<std::wstring> args;
    int argCount;
    LPWSTR* pArgs = CommandLineToArgvW(cmdLine, &argCount);
    for (int i = 0; i < argCount; i++)
    {
        args.push_back(std::wstring(pArgs[i]));
        std::wcout << args[i] << '\n';
    }
    LocalFree(pArgs);

    // Create window
    DisplayWindow window(hInst, cmdLine, L"class");

    //IVirtualDesktopManager* dmanager;
    //CoCreateInstance(CLSID_VirtualDesktopManager, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&dmanager));
    //GUID guid;
    //dmanager->GetWindowDesktopId(window.GetHWNDRef(), &guid);

    // Load resources
    ResourceManager::Init("Resources/Images/resources.resc", window.gfx.GetGraphics().target);

    // Init app
    App::Init(window);

    // Init appropriate scenes
    App::Instance()->InitScene(ConnectionScene::StaticName(), nullptr);

    Clock msgTimer = Clock(0);

    // Start app thread
    App::Start();

    // Main window loop
    while (true)
    {
        // Check for app exit
        if (App::Exited())
            break;

        // Messages
        bool msgProcessed = window.ProcessMessages();

        window.HandleFullscreenChange();
        window.HandleCursorVisibility();

        // Limit cpu usage
        if (!msgProcessed)
        {
            // If no messages are received for 50ms or more, sleep to limit cpu usage.
            // This way we allow for full* mouse poll rate utilization when necessary.
            //
            // * the very first mouse move after a break will have a very small delay
            // which may be noticeable in certain situations (FPS games)
            msgTimer.Update();
            if (msgTimer.Now().GetTime(MILLISECONDS) >= 50)
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        else
        {
            msgTimer.Reset();
        }
        continue;
    }

    return 0;
}