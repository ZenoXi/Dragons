#pragma once

#include "Scene.h"

#include "Components/Base/ComponentBase.h"
#include "Components/Base/Label.h"
#include "Components/Base/Panel.h"
#include "Components/Base/Button.h"

#include "Helper/NetBase2.h"
#include "Helper/Time.h"

struct ConnectionSceneOptions : public SceneOptionsBase
{

};

class ConnectionScene : public Scene
{
    znet::TCPClient _client;
    znet::TCPServer _server;

    int64_t _clientId;
    uint32_t _gameSeed;
    int _playerIndex;
    int _opponentIndex;

    TimePoint _startTime;
    bool _serverMode = false;
    bool _player2Mode = false;
    bool _localhost = false;
    bool _skipIntro = false;
    bool _setUpDone = false;

    Scene* _introScene = nullptr;
    bool _readyPacketSent = false;

public:
    ConnectionScene(App* app);

    const char* GetName() const { return "connection"; }
    static const char* StaticName() { return "connection"; }

private:
    void _Init(const SceneOptionsBase* options);
    void _Uninit();
    void _Focus();
    void _Unfocus();
    void _Update();
    void _Resize(int width, int height);
};
