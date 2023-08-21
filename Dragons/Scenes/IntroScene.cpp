#include "App.h" // App.h must be included first
#include "IntroScene.h"

#include "Helper/Navigation.h"

IntroScene::IntroScene(App* app)
    : Scene(app)
{}

void IntroScene::_Init(const SceneOptionsBase* options)
{
    IntroSceneOptions opt;
    if (options)
    {
        opt = *reinterpret_cast<const IntroSceneOptions*>(options);
    }

    _offensePanel = Create<zcom::EmptyPanel>();
    _offensePanel->SetBaseSize(initialBorderLength, initialBorderLength);
    _offensePanel->SetBackgroundColor(D2D1::ColorF(0xf65314, 1.0f));
    _offensePanel->SetOpacity(0.0f);

    _defensePanel = Create<zcom::EmptyPanel>();
    _defensePanel->SetBaseSize(initialBorderLength, initialBorderLength);
    _defensePanel->SetBackgroundColor(D2D1::ColorF(0x7cbb00, 1.0f));
    _defensePanel->SetOpacity(0.0f);

    _utilityPanel = Create<zcom::EmptyPanel>();
    _utilityPanel->SetBaseSize(initialBorderLength, initialBorderLength);
    _utilityPanel->SetBackgroundColor(D2D1::ColorF(0x00a1f1, 1.0f));
    _utilityPanel->SetOpacity(0.0f);

    _comboPanel = Create<zcom::EmptyPanel>();
    _comboPanel->SetBaseSize(initialBorderLength, initialBorderLength);
    _comboPanel->SetBackgroundColor(D2D1::ColorF(0xffbb00, 1.0f));
    _comboPanel->SetOpacity(0.0f);

    ztime::clock[CLOCK_MAIN].Update();
    _initTime = ztime::Main();
    _completed = false;
    _canvas->AddComponent(_offensePanel.get());
    _canvas->AddComponent(_defensePanel.get());
    _canvas->AddComponent(_utilityPanel.get());
    _canvas->AddComponent(_comboPanel.get());
    _canvas->SetBackgroundColor(D2D1::ColorF(0, 1.0f));

    float viewWidth = _canvas->GetWidth();
    float viewHeight = _canvas->GetHeight();
    Pos2D<float> centerPos = { viewWidth / 2, viewHeight / 2 };
    Pos2D<float> offenseStartPos = centerPos + Pos2D{ -(initialBorderLength + 10.0f) / 2, -(initialBorderLength + 10.0f) / 2 };
    Pos2D<float> defenseStartPos = centerPos + Pos2D{ (initialBorderLength + 10.0f) / 2, -(initialBorderLength + 10.0f) / 2 };
    Pos2D<float> utilityStartPos = centerPos + Pos2D{ -(initialBorderLength + 10.0f) / 2, (initialBorderLength + 10.0f) / 2 };
    Pos2D<float> comboStartPos = centerPos + Pos2D{ (initialBorderLength + 10.0f) / 2, (initialBorderLength + 10.0f) / 2 };
    _offensePanel->SetHorizontalOffsetPixels(offenseStartPos.x - _offensePanel->GetBaseWidth() / 2.0f);
    _offensePanel->SetVerticalOffsetPixels(offenseStartPos.y - _offensePanel->GetBaseHeight() / 2.0f);
    _defensePanel->SetHorizontalOffsetPixels(defenseStartPos.x - _defensePanel->GetBaseWidth() / 2.0f);
    _defensePanel->SetVerticalOffsetPixels(defenseStartPos.y - _defensePanel->GetBaseHeight() / 2.0f);
    _utilityPanel->SetHorizontalOffsetPixels(utilityStartPos.x - _utilityPanel->GetBaseWidth() / 2.0f);
    _utilityPanel->SetVerticalOffsetPixels(utilityStartPos.y - _utilityPanel->GetBaseHeight() / 2.0f);
    _comboPanel->SetHorizontalOffsetPixels(comboStartPos.x - _comboPanel->GetBaseWidth() / 2.0f);
    _comboPanel->SetVerticalOffsetPixels(comboStartPos.y - _comboPanel->GetBaseHeight() / 2.0f);
}

void IntroScene::_Uninit()
{
    _canvas->ClearComponents();
}

void IntroScene::_Focus()
{

}

void IntroScene::_Unfocus()
{

}

void IntroScene::_Update()
{
    Duration initialWaitDuration = Duration(2, SECONDS);
    Duration logoFadeInDuration = Duration(1, SECONDS);
    Duration logoWaitDuration = Duration(2, SECONDS);
    Duration deckMoveDuration = Duration(4, SECONDS);
    Duration gapBetweenDecksDuration = Duration(500, MILLISECONDS);
    Duration afterDeckMoveWaitDuration = Duration(1, SECONDS);
    Duration afterComboSwitchDuration = Duration(1, SECONDS);
    Duration fadeInDuration = Duration(3, SECONDS);

    Duration PHASE_1 = initialWaitDuration;
    Duration PHASE_2 = PHASE_1 + logoFadeInDuration;
    Duration PHASE_3 = PHASE_2 + logoWaitDuration;
    Duration PHASE_4 = PHASE_3 + deckMoveDuration;
    Duration PHASE_5 = PHASE_4 + afterDeckMoveWaitDuration;
    Duration PHASE_6 = PHASE_5 + afterComboSwitchDuration;
    Duration PHASE_7 = PHASE_6 + fadeInDuration;

    float viewWidth = _canvas->GetWidth();
    float viewHeight = _canvas->GetHeight();
    Pos2D<float> centerPos = { viewWidth / 2, viewHeight / 2 };

    Pos2D<float> offenseStartPos = centerPos + Pos2D{ -(initialBorderLength + 10.0f) / 2, -(initialBorderLength + 10.0f) / 2 };
    Pos2D<float> defenseStartPos = centerPos + Pos2D{ (initialBorderLength + 10.0f) / 2, -(initialBorderLength + 10.0f) / 2 };
    Pos2D<float> utilityStartPos = centerPos + Pos2D{ -(initialBorderLength + 10.0f) / 2, (initialBorderLength + 10.0f) / 2 };
    Pos2D<float> comboStartPos = centerPos + Pos2D{ (initialBorderLength + 10.0f) / 2, (initialBorderLength + 10.0f) / 2 };

    float gapFromScreenEdge = 100.0f;
    float gapBetweenDecks = 30.0f;
    float CARD_WIDTH = 180.0f;
    float CARD_HEIGHT = 240.0f;
    Pos2D<float> offenseEndPos = Pos2D{ gapFromScreenEdge + CARD_HEIGHT / 2 - (29 * 1.25f), viewHeight / 2 - gapBetweenDecks * 1.5f - CARD_WIDTH * 1.5f };
    Pos2D<float> defenseEndPos = Pos2D{ gapFromScreenEdge + CARD_HEIGHT / 2 - (29 * 1.25f), viewHeight / 2 - gapBetweenDecks * 0.5f - CARD_WIDTH * 0.5f };
    Pos2D<float> utilityEndPos = Pos2D{ gapFromScreenEdge + CARD_HEIGHT / 2 - (29 * 1.25f), viewHeight / 2 + gapBetweenDecks * 0.5f + CARD_WIDTH * 0.5f };
    Pos2D<float> comboEndPos = Pos2D{ gapFromScreenEdge + CARD_HEIGHT / 2 - (29 * 1.25f), viewHeight / 2 + gapBetweenDecks * 1.5f + CARD_WIDTH * 1.5f };

    Duration elapsedTime = ztime::Main() - _initTime;
    if (elapsedTime < PHASE_1)
    {
        // Wait
    }
    else if (elapsedTime < PHASE_2)
    {
        float delta = (elapsedTime - PHASE_1).GetDuration(MILLISECONDS) / (float)logoFadeInDuration.GetDuration(MILLISECONDS);
        _offensePanel->SetOpacity(delta);
        _defensePanel->SetOpacity(delta);
        _utilityPanel->SetOpacity(delta);
        _comboPanel->SetOpacity(delta);
    }
    else if (elapsedTime < PHASE_3)
    {
        float delta = (elapsedTime - PHASE_2).GetDuration(MILLISECONDS) / (float)logoWaitDuration.GetDuration(MILLISECONDS);
        _offensePanel->SetOpacity(1.0f);
        _defensePanel->SetOpacity(1.0f);
        _utilityPanel->SetOpacity(1.0f);
        _comboPanel->SetOpacity(1.0f);
    }
    else if (elapsedTime < PHASE_4)
    {
        float delta = (elapsedTime - PHASE_3).GetDuration(MILLISECONDS) / (float)deckMoveDuration.GetDuration(MILLISECONDS);
        float deltaOffset = gapBetweenDecksDuration.GetDuration(MILLISECONDS) / (float)deckMoveDuration.GetDuration(MILLISECONDS);

        float deltaOffense;
        float deltaDefense;
        float deltaUtility;
        float deltaCombo;

        if (delta <= deltaOffset * 0) deltaOffense = 0.0f;
        else if (delta >= 1.0f - deltaOffset * 3) deltaOffense = 1.0f;
        else deltaOffense = (delta - deltaOffset * 0) / (1.0f - deltaOffset * 3);

        if (delta <= deltaOffset * 1) deltaDefense = 0.0f;
        else if (delta >= 1.0f - deltaOffset * 2) deltaDefense = 1.0f;
        else deltaDefense = (delta - deltaOffset * 1) / (1.0f - deltaOffset * 3);

        if (delta <= deltaOffset * 2) deltaUtility = 0.0f;
        else if (delta >= 1.0f - deltaOffset * 1) deltaUtility = 1.0f;
        else deltaUtility = (delta - deltaOffset * 2) / (1.0f - deltaOffset * 3);

        if (delta <= deltaOffset * 3) deltaCombo = 0.0f;
        else if (delta >= 1.0f - deltaOffset * 0) deltaCombo = 1.0f;
        else deltaCombo = (delta - deltaOffset * 3) / (1.0f - deltaOffset * 3);

        deltaOffense = (std::cosf(deltaOffense * Math::PI + Math::PI) / 2 + 0.5f);
        deltaDefense = std::cosf(deltaDefense * Math::PI + Math::PI) / 2 + 0.5f;
        deltaUtility = std::cosf(deltaUtility * Math::PI + Math::PI) / 2 + 0.5f;
        deltaCombo = std::cosf(deltaCombo * Math::PI + Math::PI) / 2 + 0.5f;

        _offensePanel->SetBaseWidth(initialBorderLength + (CARD_HEIGHT - initialBorderLength) * deltaOffense);
        _offensePanel->SetBaseHeight(initialBorderLength + (CARD_WIDTH - initialBorderLength) * deltaOffense);
        _offensePanel->SetHorizontalOffsetPixels((offenseStartPos + (offenseEndPos - offenseStartPos) * deltaOffense).x - _offensePanel->GetBaseWidth() / 2.0f);
        _offensePanel->SetVerticalOffsetPixels((offenseStartPos + (offenseEndPos - offenseStartPos) * deltaOffense).y - _offensePanel->GetBaseHeight() / 2.0f);

        _defensePanel->SetBaseWidth(initialBorderLength + (CARD_HEIGHT - initialBorderLength) * deltaDefense);
        _defensePanel->SetBaseHeight(initialBorderLength + (CARD_WIDTH - initialBorderLength) * deltaDefense);
        _defensePanel->SetHorizontalOffsetPixels((defenseStartPos + (defenseEndPos - defenseStartPos) * deltaDefense).x - _defensePanel->GetBaseWidth() / 2.0f);
        _defensePanel->SetVerticalOffsetPixels((defenseStartPos + (defenseEndPos - defenseStartPos) * deltaDefense).y - _defensePanel->GetBaseHeight() / 2.0f);

        _utilityPanel->SetBaseWidth(initialBorderLength + (CARD_HEIGHT - initialBorderLength) * deltaUtility);
        _utilityPanel->SetBaseHeight(initialBorderLength + (CARD_WIDTH - initialBorderLength) * deltaUtility);
        _utilityPanel->SetHorizontalOffsetPixels((utilityStartPos + (utilityEndPos - utilityStartPos) * deltaUtility).x - _utilityPanel->GetBaseWidth() / 2.0f);
        _utilityPanel->SetVerticalOffsetPixels((utilityStartPos + (utilityEndPos - utilityStartPos) * deltaUtility).y - _utilityPanel->GetBaseHeight() / 2.0f);

        _comboPanel->SetBaseWidth(initialBorderLength + (CARD_HEIGHT - initialBorderLength) * deltaCombo);
        _comboPanel->SetBaseHeight(initialBorderLength + (CARD_WIDTH - initialBorderLength) * deltaCombo);
        _comboPanel->SetHorizontalOffsetPixels((comboStartPos + (comboEndPos - comboStartPos) * deltaCombo).x - _comboPanel->GetBaseWidth() / 2.0f);
        _comboPanel->SetVerticalOffsetPixels((comboStartPos + (comboEndPos - comboStartPos) * deltaCombo).y - _comboPanel->GetBaseHeight() / 2.0f);
    }
    else if (elapsedTime < PHASE_5)
    {
        float delta = (elapsedTime - PHASE_4).GetDuration(MILLISECONDS) / (float)afterDeckMoveWaitDuration.GetDuration(MILLISECONDS);
        
        _offensePanel->SetBaseWidth(CARD_HEIGHT);
        _offensePanel->SetBaseHeight(CARD_WIDTH);
        _offensePanel->SetHorizontalOffsetPixels(offenseEndPos.x - CARD_HEIGHT / 2.0f);
        _offensePanel->SetVerticalOffsetPixels(offenseEndPos.y - CARD_WIDTH / 2.0f);

        _defensePanel->SetBaseWidth(CARD_HEIGHT);
        _defensePanel->SetBaseHeight(CARD_WIDTH);
        _defensePanel->SetHorizontalOffsetPixels(defenseEndPos.x - CARD_HEIGHT / 2.0f);
        _defensePanel->SetVerticalOffsetPixels(defenseEndPos.y - CARD_WIDTH / 2.0f);

        _utilityPanel->SetBaseWidth(CARD_HEIGHT);
        _utilityPanel->SetBaseHeight(CARD_WIDTH);
        _utilityPanel->SetHorizontalOffsetPixels(utilityEndPos.x - CARD_HEIGHT / 2.0f);
        _utilityPanel->SetVerticalOffsetPixels(utilityEndPos.y - CARD_WIDTH / 2.0f);

        _comboPanel->SetBaseWidth(CARD_HEIGHT);
        _comboPanel->SetBaseHeight(CARD_WIDTH);
        _comboPanel->SetHorizontalOffsetPixels(comboEndPos.x - CARD_HEIGHT / 2.0f);
        _comboPanel->SetVerticalOffsetPixels(comboEndPos.y - CARD_WIDTH / 2.0f);
    }
    else if (elapsedTime < PHASE_6)
    {
        float delta = (elapsedTime - PHASE_5).GetDuration(MILLISECONDS) / (float)afterComboSwitchDuration.GetDuration(MILLISECONDS);

        if (delta < 0.05f)
            _comboPanel->SetBackgroundColor(D2D1::ColorF(0xffbb00, 0.9f));
        else if (delta < 0.07f)
            _comboPanel->SetBackgroundColor(D2D1::ColorF(0xffbb00));
        else if (delta < 0.12f)
            _comboPanel->SetBackgroundColor(D2D1::ColorF(0.18f, 0.17f, 0.15f));
        else if (delta < 0.14f)
            _comboPanel->SetBackgroundColor(D2D1::ColorF(0xffbb00, 0.9f));
        else if (delta < 0.24f)
            _comboPanel->SetBackgroundColor(D2D1::ColorF(0.18f, 0.17f, 0.15f));
        else if (delta < 0.26f)
            _comboPanel->SetBackgroundColor(D2D1::ColorF(0xffbb00, 0.4f));
        else
            _comboPanel->SetBackgroundColor(D2D1::ColorF(0.18f, 0.17f, 0.15f));
    }
    else if (elapsedTime < PHASE_7)
    {
        float delta = (elapsedTime - PHASE_6).GetDuration(MILLISECONDS) / (float)fadeInDuration.GetDuration(MILLISECONDS);

        _completed = true;
        _canvas->SetBackgroundColor(D2D1::ColorF(0, 1.0f - delta));
        _offensePanel->SetOpacity(1.0f - delta);
        _defensePanel->SetOpacity(1.0f - delta);
        _utilityPanel->SetOpacity(1.0f - delta);
        _comboPanel->SetOpacity(1.0f - delta);
    }
    else
    {
        _app->UninitScene(GetName());
    }
}

void IntroScene::_Resize(int width, int height)
{
    float viewWidth = width;
    float viewHeight = height;
    Pos2D<float> centerPos = { viewWidth / 2, viewHeight / 2 };
    Pos2D<float> offenseStartPos = centerPos + Pos2D{ -(initialBorderLength + 10.0f) / 2, -(initialBorderLength + 10.0f) / 2 };
    Pos2D<float> defenseStartPos = centerPos + Pos2D{ (initialBorderLength + 10.0f) / 2, -(initialBorderLength + 10.0f) / 2 };
    Pos2D<float> utilityStartPos = centerPos + Pos2D{ -(initialBorderLength + 10.0f) / 2, (initialBorderLength + 10.0f) / 2 };
    Pos2D<float> comboStartPos = centerPos + Pos2D{ (initialBorderLength + 10.0f) / 2, (initialBorderLength + 10.0f) / 2 };
    _offensePanel->SetHorizontalOffsetPixels(offenseStartPos.x - _offensePanel->GetBaseWidth() / 2.0f);
    _offensePanel->SetVerticalOffsetPixels(offenseStartPos.y - _offensePanel->GetBaseHeight() / 2.0f);
    _defensePanel->SetHorizontalOffsetPixels(defenseStartPos.x - _defensePanel->GetBaseWidth() / 2.0f);
    _defensePanel->SetVerticalOffsetPixels(defenseStartPos.y - _defensePanel->GetBaseHeight() / 2.0f);
    _utilityPanel->SetHorizontalOffsetPixels(utilityStartPos.x - _utilityPanel->GetBaseWidth() / 2.0f);
    _utilityPanel->SetVerticalOffsetPixels(utilityStartPos.y - _utilityPanel->GetBaseHeight() / 2.0f);
    _comboPanel->SetHorizontalOffsetPixels(comboStartPos.x - _comboPanel->GetBaseWidth() / 2.0f);
    _comboPanel->SetVerticalOffsetPixels(comboStartPos.y - _comboPanel->GetBaseHeight() / 2.0f);
}