#include "App.h" // App.h must be included first
#include "ConnectionScene.h"
#include "EntryScene.h"

#include "Core/GameConstants.h"
#include "Helper/Functions.h"
#include "Helper/ListUtils.h"
#include "Helper/PacketTypes.h"

#include <fstream>
#include <filesystem>

ConnectionScene::ConnectionScene(App* app)
    : Scene(app)
{}

void ConnectionScene::_Init(const SceneOptionsBase* options)
{
    ConnectionSceneOptions opt;
    if (options)
    {
        opt = *reinterpret_cast<const ConnectionSceneOptions*>(options);
    }

    _startTime = ztime::Main();
    _canvas->SetBackgroundColor(D2D1::ColorF(0));
}

void ConnectionScene::_Uninit()
{
    _canvas->ClearComponents();
}

void ConnectionScene::_Focus()
{

}

void ConnectionScene::_Unfocus()
{

}

void ConnectionScene::_Update()
{
    _canvas->Update();

    // Wait 2 seconds for client/server choice
    if ((ztime::Main() - _startTime).GetDuration(SECONDS) < 2)
    {
        if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000 && GetAsyncKeyState(VK_NUMPAD9) & 0x8000)
            _serverMode = true;
        if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000 && GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
            _player2Mode = true;
        return;
    }

    if (!_setUpDone)
    {
        if (_serverMode)
        {
            _server.StartServer(54000);
            _server.AllowNewConnections();
            while (_server.NewConnectionCount() == 0)
                std::this_thread::sleep_for(std::chrono::milliseconds(50));

            _clientId = _server.GetNewConnection();
            znet::TCPClientRef ref = _server.Connection(_clientId);

            // Generate and send game seed
            std::random_device dev;
            _gameSeed = dev();
            ref->Send(znet::Packet((int)PacketType::GAME_SEED).From(_gameSeed));

            _playerIndex = _player2Mode ? (int)1 : (int)0;
            _opponentIndex = _playerIndex == 0 ? 1 : 0;
            // Send opponent player index
            ref->Send(znet::Packet((int)PacketType::PLAYER_INDEX).From(_opponentIndex));

            // Wait for game ready signal
            while (ref->PacketCount() == 0)
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            ref->GetPacket();

            EntrySceneOptions opt;
            opt.connection = ref;
            opt.gameSeed = _gameSeed;
            opt.playerIndex = _playerIndex;
            opt.opponentIndex = _opponentIndex;
            _app->InitScene(EntryScene::StaticName(), &opt);
            _app->MoveSceneToFront(EntryScene::StaticName());
        }
        else
        {
            _clientId = _client.Connect("127.0.0.1", 54000);
            znet::TCPClientRef ref = _client.Connection(_clientId);

            while (ref->PacketCount() < 2)
                std::this_thread::sleep_for(std::chrono::milliseconds(50));

            _gameSeed = ref->GetPacket().Cast<uint32_t>();
            _playerIndex = ref->GetPacket().Cast<int>();
            _opponentIndex = _playerIndex == 0 ? 1 : 0;

            ref->Send(znet::Packet((int)PacketType::CLIENT_READY).From((int)1));

            EntrySceneOptions opt;
            opt.connection = ref;
            opt.gameSeed = _gameSeed;
            opt.playerIndex = _playerIndex;
            opt.opponentIndex = _opponentIndex;
            _app->InitScene(EntryScene::StaticName(), &opt);
            _app->MoveSceneToFront(EntryScene::StaticName());
        }

        _setUpDone = true;
    }
}

void ConnectionScene::_Resize(int width, int height)
{

}