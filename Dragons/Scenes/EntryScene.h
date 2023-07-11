#pragma once

#include "Scene.h"

#include "Components/Base/ComponentBase.h"
#include "Components/Base/Label.h"

#include "Components/Custom/Board.h"

#include "Core/Core.h"

struct EntrySceneOptions : public SceneOptionsBase
{

};

class EntryScene : public Scene
{
    std::unique_ptr<zcom::Label> _helloWorldLabel = nullptr;
    std::unique_ptr<zcom::Board> _board = nullptr;

    Core core;

public:
    EntryScene(App* app);

    const char* GetName() const { return "entry"; }
    static const char* StaticName() { return "entry"; }

private:
    void _Init(const SceneOptionsBase* options);
    void _Uninit();
    void _Focus();
    void _Unfocus();
    void _Update();
    void _Resize(int width, int height);
};
