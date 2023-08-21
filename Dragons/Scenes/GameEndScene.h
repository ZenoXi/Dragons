#pragma once

#include "Scene.h"

#include "Components/Base/ComponentBase.h"
#include "Components/Base/Label.h"
#include "Components/Base/Panel.h"
#include "Components/Base/Button.h"

#include "Helper/Time.h"

struct GameEndSceneOptions : public SceneOptionsBase
{
    bool victory;
};

class GameEndScene : public Scene
{
    bool _victory;
    TimePoint _initTime;

    bool _fadeOutDone = false;
    std::unique_ptr<zcom::Label> _endLabel = nullptr;

public:
    GameEndScene(App* app);

    const char* GetName() const { return "game_end"; }
    static const char* StaticName() { return "game_end"; }

private:
    void _Init(const SceneOptionsBase* options);
    void _Uninit();
    void _Focus();
    void _Unfocus();
    void _Update();
    void _Resize(int width, int height);
    bool _Redraw() { return true; }
};
