#include "App.h" // App.h must be included first
#include "EntryScene.h"

#include "Helper/Functions.h"

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

    _p1StatsPanel = Create<zcom::Panel>();
    _p1StatsPanel->SetBaseSize(300, 120);
    _p1StatsPanel->SetOffsetPixels(-35, -25);
    _p1StatsPanel->SetAlignment(zcom::Alignment::END, zcom::Alignment::END);

    _p1HealthLabel = Create<zcom::Label>(L"Health:");
    _p1HealthLabel->SetVerticalOffsetPixels(0);
    _p1HealthLabel->SetBaseSize(250, 40);
    _p1HealthLabel->SetFont(L"Arial");
    _p1HealthLabel->SetFontSize(36.0f);
    _p1HealthLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

    _p1MaxHealthLabel = Create<zcom::Label>(L"Max health:");
    _p1MaxHealthLabel->SetVerticalOffsetPixels(40);
    _p1MaxHealthLabel->SetBaseSize(250, 40);
    _p1MaxHealthLabel->SetFont(L"Arial");
    _p1MaxHealthLabel->SetFontSize(36.0f);
    _p1MaxHealthLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

    _p1ArmorLabel = Create<zcom::Label>(L"Armor:");
    _p1ArmorLabel->SetVerticalOffsetPixels(80);
    _p1ArmorLabel->SetBaseSize(250, 40);
    _p1ArmorLabel->SetFont(L"Arial");
    _p1ArmorLabel->SetFontSize(36.0f);
    _p1ArmorLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

    _p1HealthAmountLabel = Create<zcom::Label>(L"10");
    _p1HealthAmountLabel->SetHorizontalAlignment(zcom::Alignment::END);
    _p1HealthAmountLabel->SetHorizontalTextAlignment(zcom::TextAlignment::TRAILING);
    _p1HealthAmountLabel->SetVerticalOffsetPixels(0);
    _p1HealthAmountLabel->SetBaseSize(50, 40);
    _p1HealthAmountLabel->SetFont(L"Arial");
    _p1HealthAmountLabel->SetFontSize(36.0f);
    _p1HealthAmountLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

    _p1MaxHealthAmountLabel = Create<zcom::Label>(L"10");
    _p1MaxHealthAmountLabel->SetHorizontalAlignment(zcom::Alignment::END);
    _p1MaxHealthAmountLabel->SetHorizontalTextAlignment(zcom::TextAlignment::TRAILING);
    _p1MaxHealthAmountLabel->SetVerticalOffsetPixels(40);
    _p1MaxHealthAmountLabel->SetBaseSize(50, 40);
    _p1MaxHealthAmountLabel->SetFont(L"Arial");
    _p1MaxHealthAmountLabel->SetFontSize(36.0f);
    _p1MaxHealthAmountLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

    _p1ArmorAmountLabel = Create<zcom::Label>(L"0");
    _p1ArmorAmountLabel->SetHorizontalAlignment(zcom::Alignment::END);
    _p1ArmorAmountLabel->SetHorizontalTextAlignment(zcom::TextAlignment::TRAILING);
    _p1ArmorAmountLabel->SetVerticalOffsetPixels(80);
    _p1ArmorAmountLabel->SetBaseSize(50, 40);
    _p1ArmorAmountLabel->SetFont(L"Arial");
    _p1ArmorAmountLabel->SetFontSize(36.0f);
    _p1ArmorAmountLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

    _p1StatsPanel->AddItem(_p1HealthLabel.get());
    _p1StatsPanel->AddItem(_p1MaxHealthLabel.get());
    _p1StatsPanel->AddItem(_p1ArmorLabel.get());
    _p1StatsPanel->AddItem(_p1HealthAmountLabel.get());
    _p1StatsPanel->AddItem(_p1MaxHealthAmountLabel.get());
    _p1StatsPanel->AddItem(_p1ArmorAmountLabel.get());

    _p2StatsPanel = Create<zcom::Panel>();
    _p2StatsPanel->SetBaseSize(300, 120);
    _p2StatsPanel->SetOffsetPixels(-35, 25);
    _p2StatsPanel->SetAlignment(zcom::Alignment::END, zcom::Alignment::START);

    _p2HealthLabel = Create<zcom::Label>(L"Health:");
    _p2HealthLabel->SetVerticalOffsetPixels(0);
    _p2HealthLabel->SetBaseSize(250, 40);
    _p2HealthLabel->SetFont(L"Arial");
    _p2HealthLabel->SetFontSize(36.0f);
    _p2HealthLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

    _p2MaxHealthLabel = Create<zcom::Label>(L"Max health:");
    _p2MaxHealthLabel->SetVerticalOffsetPixels(40);
    _p2MaxHealthLabel->SetBaseSize(250, 40);
    _p2MaxHealthLabel->SetFont(L"Arial");
    _p2MaxHealthLabel->SetFontSize(36.0f);
    _p2MaxHealthLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

    _p2ArmorLabel = Create<zcom::Label>(L"Armor:");
    _p2ArmorLabel->SetVerticalOffsetPixels(80);
    _p2ArmorLabel->SetBaseSize(250, 40);
    _p2ArmorLabel->SetFont(L"Arial");
    _p2ArmorLabel->SetFontSize(36.0f);
    _p2ArmorLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

    _p2HealthAmountLabel = Create<zcom::Label>(L"10");
    _p2HealthAmountLabel->SetHorizontalAlignment(zcom::Alignment::END);
    _p2HealthAmountLabel->SetHorizontalTextAlignment(zcom::TextAlignment::TRAILING);
    _p2HealthAmountLabel->SetVerticalOffsetPixels(0);
    _p2HealthAmountLabel->SetBaseSize(50, 40);
    _p2HealthAmountLabel->SetFont(L"Arial");
    _p2HealthAmountLabel->SetFontSize(36.0f);
    _p2HealthAmountLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
    
    _p2MaxHealthAmountLabel = Create<zcom::Label>(L"10");
    _p2MaxHealthAmountLabel->SetHorizontalAlignment(zcom::Alignment::END);
    _p2MaxHealthAmountLabel->SetHorizontalTextAlignment(zcom::TextAlignment::TRAILING);
    _p2MaxHealthAmountLabel->SetVerticalOffsetPixels(40);
    _p2MaxHealthAmountLabel->SetBaseSize(50, 40);
    _p2MaxHealthAmountLabel->SetFont(L"Arial");
    _p2MaxHealthAmountLabel->SetFontSize(36.0f);
    _p2MaxHealthAmountLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
    
    _p2ArmorAmountLabel = Create<zcom::Label>(L"0");
    _p2ArmorAmountLabel->SetHorizontalAlignment(zcom::Alignment::END);
    _p2ArmorAmountLabel->SetHorizontalTextAlignment(zcom::TextAlignment::TRAILING);
    _p2ArmorAmountLabel->SetVerticalOffsetPixels(80);
    _p2ArmorAmountLabel->SetBaseSize(50, 40);
    _p2ArmorAmountLabel->SetFont(L"Arial");
    _p2ArmorAmountLabel->SetFontSize(36.0f);
    _p2ArmorAmountLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

    _p2StatsPanel->AddItem(_p1HealthLabel.get());
    _p2StatsPanel->AddItem(_p1MaxHealthLabel.get());
    _p2StatsPanel->AddItem(_p1ArmorLabel.get());
    _p2StatsPanel->AddItem(_p1HealthAmountLabel.get());
    _p2StatsPanel->AddItem(_p1MaxHealthAmountLabel.get());
    _p2StatsPanel->AddItem(_p1ArmorAmountLabel.get());

    _p1ExtraActionsLabel = Create<zcom::Label>(L"Actions left: 2\nExtra actions: 3\n[Play] -\n[Play] -\n[Play or Draw] -");
    _p1ExtraActionsLabel->SetAlignment(zcom::Alignment::END, zcom::Alignment::END);
    _p1ExtraActionsLabel->SetVerticalTextAlignment(zcom::Alignment::END);
    _p1ExtraActionsLabel->SetHorizontalTextAlignment(zcom::TextAlignment::TRAILING);
    _p1ExtraActionsLabel->SetBaseSize(300, 250);
    _p1ExtraActionsLabel->SetOffsetPixels(-40, -160);
    _p1ExtraActionsLabel->SetFont(L"Arial");
    _p1ExtraActionsLabel->SetFontSize(24.0f);
    _p1ExtraActionsLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

    _p2ExtraActionsLabel = Create<zcom::Label>(L"Actions left: 2\nExtra actions: 3\n[Play] -\n[Play] -\n[Play or Draw] -");
    _p2ExtraActionsLabel->SetAlignment(zcom::Alignment::END, zcom::Alignment::START);
    _p2ExtraActionsLabel->SetVerticalTextAlignment(zcom::Alignment::START);
    _p2ExtraActionsLabel->SetHorizontalTextAlignment(zcom::TextAlignment::TRAILING);
    _p2ExtraActionsLabel->SetBaseSize(300, 250);
    _p2ExtraActionsLabel->SetOffsetPixels(-40, 160);
    _p2ExtraActionsLabel->SetFont(L"Arial");
    _p2ExtraActionsLabel->SetFontSize(24.0f);
    _p2ExtraActionsLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

    _canvas->AddComponent(_board.get());
    //_canvas->AddComponent(_p1StatsPanel.get());
    //_canvas->AddComponent(_p2StatsPanel.get());
    //_canvas->AddComponent(_p1ExtraActionsLabel.get());
    //_canvas->AddComponent(_p2ExtraActionsLabel.get());
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
    _canvas->Update();

    { // Update player 1 stat labels
        auto& player = core.GetState().players[0];

        std::wostringstream ss(L"");
        ss << L"Actions left: " << player.actionsLeft;
        if (!player.extraActions.empty())
        {
            ss << '\n' << L"Extra actions: " << player.extraActions.size() << '\n';
            for (auto& action : player.extraActions)
            {
                if (action.play && action.draw)
                    ss << L"[Play or Draw] -\n";
                else if (action.play)
                    ss << L"[Play] -\n";
                else if (action.draw)
                    ss << L"[Draw] -\n";
            }
        }
        _p1ExtraActionsLabel->SetText(ss.str());

        ss.str(L"");
        ss.clear();
        ss << player.health;
        _p1HealthAmountLabel->SetText(ss.str());
        Color healthColor = HSVtoRGB(120, 100, 85);
        if (player.health < 10)
            healthColor = HSVtoRGB(120 * (player.health / 10.0f), 100, 85);
        _p1HealthAmountLabel->SetFontColor(D2D1::ColorF(healthColor.r, healthColor.g, healthColor.b));

        ss.str(L"");
        ss.clear();
        ss << player.maxHealth;
        _p1MaxHealthAmountLabel->SetText(ss.str());

        ss.str(L"");
        ss.clear();
        ss << player.armor;
        _p1ArmorAmountLabel->SetText(ss.str());
    }
    { // Update player 2 stat labels
        auto& player = core.GetState().players[1];

        std::wostringstream ss(L"");
        ss << L"Actions left: " << player.actionsLeft << '\n';
        if (!player.extraActions.empty())
        {
            ss << L"Extra actions: " << player.extraActions.size() << '\n';
            for (auto& action : player.extraActions)
            {
                if (action.play && action.draw)
                    ss << L"[Play or Draw] -\n";
                else if (action.play)
                    ss << L"[Play] -\n";
                else if (action.draw)
                    ss << L"[Draw] -\n";
            }
        }
        _p2ExtraActionsLabel->SetText(ss.str());

        ss.str(L"");
        ss.clear();
        ss << player.health;
        _p2HealthAmountLabel->SetText(ss.str());
        Color healthColor = HSVtoRGB(120, 100, 85);
        if (player.health < 10)
            healthColor = HSVtoRGB(120 * (player.health / 10.0f), 100, 85);
        _p2HealthAmountLabel->SetFontColor(D2D1::ColorF(healthColor.r, healthColor.g, healthColor.b));

        ss.str(L"");
        ss.clear();
        ss << player.maxHealth;
        _p2MaxHealthAmountLabel->SetText(ss.str());

        ss.str(L"");
        ss.clear();
        ss << player.armor;
        _p2ArmorAmountLabel->SetText(ss.str());
    }
}

void EntryScene::_Resize(int width, int height)
{

}