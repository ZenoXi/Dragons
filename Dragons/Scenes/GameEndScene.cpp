#include "App.h" // App.h must be included first
#include "GameEndScene.h"

GameEndScene::GameEndScene(App* app)
    : Scene(app)
{}

void GameEndScene::_Init(const SceneOptionsBase* options)
{
    GameEndSceneOptions opt;
    if (options)
    {
        opt = *reinterpret_cast<const GameEndSceneOptions*>(options);
    }

    _victory = opt.victory;

    if (_victory)
        _endLabel = Create<zcom::Label>(L"VICTORY");
    else
        _endLabel = Create<zcom::Label>(L"GET FUCKING REKT");
    _endLabel->SetBaseSize(1000, 100);
    _endLabel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::CENTER);
    _endLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
    _endLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
    _endLabel->SetFont(L"Segoe UI");
    _endLabel->SetFontSize(64.0f);
    _endLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
    _endLabel->SetFontColor(D2D1::ColorF(1.0f, 0.89f, 0.5f));

    ztime::clock[CLOCK_MAIN].Update();
    _initTime = ztime::Main();
    _canvas->AddComponent(_endLabel.get());
    _canvas->SetBackgroundColor(D2D1::ColorF(0, 0.0f));
}

void GameEndScene::_Uninit()
{
    _canvas->ClearComponents();
}

void GameEndScene::_Focus()
{

}

void GameEndScene::_Unfocus()
{

}

void GameEndScene::_Update()
{
    if (!_fadeOutDone)
    {
        float fadeOutDelta = (ztime::Main() - _initTime).GetDuration(MILLISECONDS) / 500.0f;
        if (fadeOutDelta >= 1.0f)
        {
            fadeOutDelta = 1.0f;
            _fadeOutDone = true;
        }

        _canvas->SetBackgroundColor(D2D1::ColorF(0, 0.5f * fadeOutDelta));
    }

    Duration LONG_DASH_DURATION = Duration(150, MILLISECONDS);
    Duration ACCELLERATION_DURATION = Duration(50, MILLISECONDS);
    Duration SLOW_MOVE_DURATION = Duration(2, SECONDS);

    float longDashDistance = 1500.0f;
    float longDashDuration = LONG_DASH_DURATION.GetDuration(MILLISECONDS) / 1000.0f;
    float accelerationDuration = ACCELLERATION_DURATION.GetDuration(MILLISECONDS) / 1000.0f;
    float slowMoveDistance = 100.0f;
    float slowMoveDuration = SLOW_MOVE_DURATION.GetDuration(MILLISECONDS) / 1000.0f;

    float v0 = longDashDistance / longDashDuration; // Dash velocity
    float v1 = slowMoveDistance / slowMoveDuration; // Slow move velocity
    float t = accelerationDuration;
    float a = (v1 - v0) / t;
    float accelerationDistance = v0 * t + (a * t * t) / 2; // Acceleration distance

    Duration PHASE_1 = LONG_DASH_DURATION; // Fast dash from left to right
    Duration PHASE_2 = PHASE_1 + ACCELLERATION_DURATION; // Quick slow down
    Duration PHASE_3 = PHASE_2 + SLOW_MOVE_DURATION; // Slow pan across the center
    Duration PHASE_4 = PHASE_3 + ACCELLERATION_DURATION; // Quick acceleration
    Duration PHASE_5 = PHASE_4 + LONG_DASH_DURATION; // Fast dash off screen to right

    Duration elapsedTime = ztime::Main() - _initTime;
    if (elapsedTime < PHASE_1)
    {
        float delta = elapsedTime.GetDuration(MILLISECONDS) / (float)LONG_DASH_DURATION.GetDuration(MILLISECONDS);
        _endLabel->SetHorizontalOffsetPixels(-(slowMoveDistance / 2) - accelerationDistance - longDashDistance * (1.0f - delta));
    }
    else if (elapsedTime < PHASE_2)
    {
        float delta = (elapsedTime - PHASE_1).GetDuration(MILLISECONDS) / (float)ACCELLERATION_DURATION.GetDuration(MILLISECONDS);
        float dt = accelerationDuration * delta; // Elapsed seconds

        float s0 = -(slowMoveDistance / 2) - accelerationDistance; // Start position
        float s1 = s0 + v0 * dt + (a * dt * dt) / 2; // Current position
        _endLabel->SetHorizontalOffsetPixels(s1);
    }
    else if (elapsedTime < PHASE_3)
    {
        float delta = (elapsedTime - PHASE_2).GetDuration(MILLISECONDS) / (float)SLOW_MOVE_DURATION.GetDuration(MILLISECONDS);
        _endLabel->SetHorizontalOffsetPixels(-(slowMoveDistance / 2) + slowMoveDistance * delta);
    }
    else if (elapsedTime < PHASE_4)
    {
        float delta = (elapsedTime - PHASE_3).GetDuration(MILLISECONDS) / (float)ACCELLERATION_DURATION.GetDuration(MILLISECONDS);
        float dt = accelerationDuration * delta; // Elapsed seconds

        float s0 = slowMoveDistance / 2; // Start position
        float s1 = s0 + v1 * dt + (-a * dt * dt) / 2; // Current position
        _endLabel->SetHorizontalOffsetPixels(s1);
    }
    else if (elapsedTime < PHASE_5)
    {
        float delta = (elapsedTime - PHASE_4).GetDuration(MILLISECONDS) / (float)LONG_DASH_DURATION.GetDuration(MILLISECONDS);
        _endLabel->SetHorizontalOffsetPixels(slowMoveDistance / 2 + accelerationDistance + longDashDistance * delta);
    }
    else if (elapsedTime > PHASE_5 + Duration(1, SECONDS))
    {
        exit(0);
    }
}

void GameEndScene::_Resize(int width, int height)
{

}