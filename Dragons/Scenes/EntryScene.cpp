#include "App.h" // App.h must be included first
#include "EntryScene.h"

#include <fstream>
#include <filesystem>

EntryScene::EntryScene(App* app)
    : Scene(app)
{}

void EntryScene::_Init(const SceneOptionsBase* options)
{
    EntrySceneOptions opt;
    if (options)
    {
        opt = *reinterpret_cast<const EntrySceneOptions*>(options);
    }

    core.InitState();

    _helloWorldLabel = Create<zcom::Label>(L"Hello, world!");
    _helloWorldLabel->SetBaseSize(200, 50);
    _helloWorldLabel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::CENTER);
    _helloWorldLabel->SetFontSize(32.0f);
    _helloWorldLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
    _helloWorldLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
    _helloWorldLabel->SetBackgroundColor(D2D1::ColorF(0.79f, 0.22f, 0.14f));

    _board = Create<zcom::Board>(&core);
    _board->SetParentSizePercent(1.0f, 1.0f);
    zcom::PROP_Shadow shadowProp;
    shadowProp.color = D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f);
    shadowProp.blurStandardDeviation = 5.0f;
    _board->SetProperty(shadowProp);
    _board->SetBackgroundColor(D2D1::ColorF(0, 0.0f));

    _canvas->AddComponent(_board.get());
    //_canvas->AddComponent(_helloWorldLabel.get());
    _canvas->SetBackgroundColor(D2D1::ColorF(0.2f, 0.12f, 0.06f));
}

void EntryScene::_Uninit()
{
    _canvas->ClearComponents();
}

void EntryScene::_Focus()
{

}

void EntryScene::_Unfocus()
{

}

void EntryScene::_Update()
{

}

void EntryScene::_Resize(int width, int height)
{

}