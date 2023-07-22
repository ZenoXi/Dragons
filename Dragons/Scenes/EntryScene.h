#pragma once

#include "Scene.h"

#include "Components/Base/ComponentBase.h"
#include "Components/Base/Label.h"
#include "Components/Base/Panel.h"

#include "Components/Custom/Board.h"

#include "Core/Core.h"
#include "Core/Events/TurnEvents.h"

struct EntrySceneOptions : public SceneOptionsBase
{

};

class EntryScene : public Scene
{
    Core core;

    std::unique_ptr<zcom::Label> _helloWorldLabel = nullptr;
    std::unique_ptr<zcom::Board> _board = nullptr;

    std::unique_ptr<zcom::Panel> _p1StatsPanel = nullptr;
    std::unique_ptr<zcom::Label> _p1HealthLabel = nullptr;
    std::unique_ptr<zcom::Label> _p1MaxHealthLabel = nullptr;
    std::unique_ptr<zcom::Label> _p1ArmorLabel = nullptr;
    std::unique_ptr<zcom::Label> _p1HealthAmountLabel = nullptr;
    std::unique_ptr<zcom::Label> _p1MaxHealthAmountLabel = nullptr;
    std::unique_ptr<zcom::Label> _p1ArmorAmountLabel = nullptr;

    std::unique_ptr<zcom::Panel> _p2StatsPanel = nullptr;
    std::unique_ptr<zcom::Label> _p2HealthLabel = nullptr;
    std::unique_ptr<zcom::Label> _p2MaxHealthLabel = nullptr;
    std::unique_ptr<zcom::Label> _p2ArmorLabel = nullptr;
    std::unique_ptr<zcom::Label> _p2HealthAmountLabel = nullptr;
    std::unique_ptr<zcom::Label> _p2MaxHealthAmountLabel = nullptr;
    std::unique_ptr<zcom::Label> _p2ArmorAmountLabel = nullptr;

    std::unique_ptr<zcom::Label> _p1ExtraActionsLabel = nullptr;
    std::unique_ptr<zcom::Label> _p2ExtraActionsLabel = nullptr;

    // Event handlers
    std::unique_ptr<EventHandler<TurnBeginEvent>> _turnBeginHandler = nullptr;
    std::unique_ptr<EventHandler<TurnEndEvent>> _turnEndHandler = nullptr;

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
