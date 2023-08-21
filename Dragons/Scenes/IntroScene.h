#pragma once

#include "Scene.h"

#include "Components/Base/ComponentBase.h"
#include "Components/Base/Label.h"
#include "Components/Base/Panel.h"
#include "Components/Base/EmptyPanel.h"
#include "Components/Base/Button.h"

#include "Helper/Time.h"

struct IntroSceneOptions : public SceneOptionsBase
{
    bool victory;
};

class IntroScene : public Scene
{
    TimePoint _initTime;
    bool _completed = false;

    float initialBorderLength = 150.0f;
    std::unique_ptr<zcom::EmptyPanel> _offensePanel = nullptr;
    std::unique_ptr<zcom::EmptyPanel> _defensePanel = nullptr;
    std::unique_ptr<zcom::EmptyPanel> _utilityPanel = nullptr;
    std::unique_ptr<zcom::EmptyPanel> _comboPanel = nullptr;

public:
    IntroScene(App* app);

    bool Completed() { return _completed; }

    const char* GetName() const { return "intro"; }
    static const char* StaticName() { return "intro"; }

private:
    void _Init(const SceneOptionsBase* options);
    void _Uninit();
    void _Focus();
    void _Unfocus();
    void _Update();
    void _Resize(int width, int height);
    bool _Redraw() { return true; }
};
