#include "Board.h"
#include "App.h"

#include "Helper/Functions.h"
#include "Helper/ListUtils.h"
#include "Card.h"

void zcom::Board::_OnUpdate()
{
    // Resend mouse move every tick, to properly detect hovered cards while not moving the mouse
    if (GetMouseInside())
        _OnMouseMove(GetMousePosX(), GetMousePosY());

    _HandleEvents();

    // Update moving animations
    for (auto& card : _cards)
    {
        if (card.moving)
        {
            float progress = (ztime::Main() - card.moveStartTime).GetTicks() / (float)card.moveDuration.GetTicks();
            if (progress >= 1.0f)
            {
                card.moving = false;
            }
            else
            {
                progress = -std::powf(progress - 1.0f, 4) + 1.0f;
                card.xPos = card.startXPos + (card.targetXPos - card.startXPos) * progress;
                card.yPos = card.startYPos + (card.targetYPos - card.startYPos) * progress;
                card.rotation = card.startRotation + (card.targetRotation - card.startRotation) * progress;
                card.opacity = card.startOpacity + (card.targetOpacity - card.startOpacity) * progress;
            }
        }

        if (!card.moving)
        {
            card.xPos = card.targetXPos;
            card.yPos = card.targetYPos;
            card.rotation = card.targetRotation;
            card.opacity = card.targetOpacity;
        }
    }
}

void zcom::Board::_OnDraw(Graphics g)
{
    float viewWidth = GetWidth();
    float viewHeight = GetHeight();

    //float padding = std::ceilf(shadowRadius);
    float padding = 0.0f;

    //if (_offenseCardBitmap)
    //    _offenseCardBitmap->Release();
    //if (_defenseCardBitmap)
    //    _defenseCardBitmap->Release();
    //if (_utilityCardBitmap)
    //    _utilityCardBitmap->Release();
    //if (_comboCardBitmap)
    //    _comboCardBitmap->Release();

    // Generate card bitmaps
    if (!_offenseCardBitmap)
        _GenerateCardBitmap(g, &_offenseCardBitmap, D2D1::ColorF(0.79f, 0.22f, 0.14f));
    if (!_defenseCardBitmap)
        _GenerateCardBitmap(g, &_defenseCardBitmap, D2D1::ColorF(0.27f, 0.70f, 0.29f));
    if (!_utilityCardBitmap)
        _GenerateCardBitmap(g, &_utilityCardBitmap, D2D1::ColorF(0.24f, 0.50f, 0.75f));
    if (!_comboCardBitmap)
        _GenerateCardBitmap(g, &_comboCardBitmap, D2D1::ColorF(0.18f, 0.17f, 0.15f));

    _CreateMissingCards();
    _CalculateCardTargetPositions();

    std::sort(_cards.begin(), _cards.end(), [](const _Card& card1, const _Card& card2) { return card1.zIndex < card2.zIndex; });
    // Move hovered or held card to end
    auto it = std::find_if(_cards.begin(), _cards.end(), [&](const _Card& card) { return card.card == _hoveredCard || card.card == _heldCard; });
    if (it != _cards.end())
    {
        _Card card = *it;
        _cards.erase(it);
        _cards.insert(_cards.end(), card);
    }
    //g.target->DrawBitmap(_offenseCardBitmap);

    ID2D1SolidColorBrush* selectedBorderBrush = nullptr;
    g.target->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 0.5f), &selectedBorderBrush);

    for (auto& card : _cards)
    {
        if (card.set.set == cards::CardSets::NONE)
            continue;

        // Generate card bitmaps
        if (!card.faceBitmap)
        {
            auto cardFaceComponent = Create<Card>(card.card);
            card.faceBitmap = cardFaceComponent->RenderCardBitmap(g);
            auto cardBackComponent = Create<Card>(card.card->GetCardType());
            card.backBitmap = cardBackComponent->RenderCardBitmap(g);
        }

        _Card copy = card;
        if (_hoveredCard == card.card && !card.moving && _hoveredCardInHand && (card.set.playerIndex == _uiState.currentPlayer || _uiState.players[card.set.playerIndex].handRevealed))
        {
            float newHeight = CARD_HEIGHT * 1.8f;
            if (card.set.playerIndex == _uiState.currentPlayer)
                card.yPos = viewHeight - newHeight / 2 - 50.0f;
            else
                card.yPos = newHeight / 2 + 50.0f;
            card.scale = 1.8f;
            card.rotation = 0.0f;
        }
        else if (_heldCard == card.card)
        {
            card.xPos = GetMousePosX();
            card.yPos = GetMousePosY();
            card.scale = 1.1f;
            card.rotation = 0.0f;
        }

        D2D1_RECT_F rect = D2D1::RectF(
            card.xPos - (CARD_WIDTH / 2 - padding) * card.scale,
            card.yPos - (CARD_HEIGHT / 2 - padding) * card.scale,
            card.xPos + (CARD_WIDTH / 2 + padding) * card.scale,
            card.yPos + (CARD_HEIGHT / 2 + padding) * card.scale
        );
        float selectedBorderThickness = 6.0f * card.scale;
        D2D1_RECT_F selectedBorderRect = D2D1::RectF(
            rect.left + selectedBorderThickness / 2,
            rect.top + selectedBorderThickness / 2,
            rect.right - selectedBorderThickness / 2,
            rect.bottom - selectedBorderThickness / 2
        );

        bool cardFaceUp =
            card.set.set == cards::CardSets::ACTIVE_CARDS ||
            (card.set.set == cards::CardSets::HAND && card.set.playerIndex == _uiState.currentPlayer) ||
            (card.set.set == cards::CardSets::HAND && _uiState.players[card.set.playerIndex].handRevealed) ||
            card.set.set == cards::CardSets::IN_PLAY;

        bool cardSelected =
            (_chooseCardsFromHandMode && _chosenCardsFromHand.find(card.card) != _chosenCardsFromHand.end()) ||
            (_chooseCardsFromActiveCardsMode && _chosenCardsFromActiveCards.find(card.card) != _chosenCardsFromActiveCards.end());

        g.target->SetTransform(D2D1::Matrix3x2F::Rotation(-card.rotation * RADIAN, { card.xPos, card.yPos }));
        if (cardFaceUp)
            g.target->DrawBitmap(card.faceBitmap, &rect, card.opacity, D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC);
        else
            g.target->DrawBitmap(card.backBitmap, &rect, card.opacity, D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC);
        if (cardSelected)
            g.target->DrawRectangle(selectedBorderRect, selectedBorderBrush, selectedBorderThickness);
        g.target->SetTransform(D2D1::Matrix3x2F::Identity());

        card = copy;
    }

    selectedBorderBrush->Release();

    // Draw stats
    //_UpdateStatLabels();

    if (_p1StatsPanel->Redraw())
        _p1StatsPanel->Draw(g);
    if (_p2StatsPanel->Redraw())
        _p2StatsPanel->Draw(g);
    if (_p1ExtraActionsLabel->Redraw())
        _p1ExtraActionsLabel->Draw(g);
    if (_p2ExtraActionsLabel->Redraw())
        _p2ExtraActionsLabel->Draw(g);

    D2D1_RECT_F p1StatsPanelRect;
    D2D1_RECT_F p2StatsPanelRect;
    D2D1_RECT_F p1ActionsLabelRect;
    D2D1_RECT_F p2ActionsLabelRect;

    if (_uiState.currentPlayer == 0)
    {
        p1StatsPanelRect = D2D1::RectF(
            viewWidth - _p1StatsPanel->GetWidth() - 35,
            viewHeight - _p1StatsPanel->GetHeight() - 25,
            viewWidth - 35,
            viewHeight - 25
        );
        p2StatsPanelRect = D2D1::RectF(
            viewWidth - _p2StatsPanel->GetWidth() - 35,
            25,
            viewWidth - 35,
            _p2StatsPanel->GetHeight() + 25
        );
        p1ActionsLabelRect = D2D1::RectF(
            viewWidth - _p1ExtraActionsLabel->GetWidth() - 40,
            viewHeight - _p1ExtraActionsLabel->GetHeight() - 160,
            viewWidth - 40,
            viewHeight - 160
        );
        p2ActionsLabelRect = D2D1::RectF(
            viewWidth - _p2ExtraActionsLabel->GetWidth() - 40,
            160,
            viewWidth - 40,
            _p2ExtraActionsLabel->GetHeight() + 160
        );

        _p1ExtraActionsLabel->SetVerticalTextAlignment(zcom::Alignment::END);
        _p2ExtraActionsLabel->SetVerticalTextAlignment(zcom::Alignment::START);
    }
    else
    {
        p2StatsPanelRect = D2D1::RectF(
            viewWidth - _p2StatsPanel->GetWidth() - 35,
            viewHeight - _p2StatsPanel->GetHeight() - 25,
            viewWidth - 35,
            viewHeight - 25
        );
        p1StatsPanelRect = D2D1::RectF(
            viewWidth - _p1StatsPanel->GetWidth() - 35,
            25,
            viewWidth - 35,
            _p1StatsPanel->GetHeight() + 25
        );
        p2ActionsLabelRect = D2D1::RectF(
            viewWidth - _p2ExtraActionsLabel->GetWidth() - 40,
            viewHeight - _p2ExtraActionsLabel->GetHeight() - 160,
            viewWidth - 40,
            viewHeight - 160
        );
        p1ActionsLabelRect = D2D1::RectF(
            viewWidth - _p1ExtraActionsLabel->GetWidth() - 40,
            160,
            viewWidth - 40,
            _p1ExtraActionsLabel->GetHeight() + 160
        );

        _p1ExtraActionsLabel->SetVerticalTextAlignment(zcom::Alignment::START);
        _p2ExtraActionsLabel->SetVerticalTextAlignment(zcom::Alignment::END);
    }
    g.target->DrawBitmap(_p1StatsPanel->Image(), p1StatsPanelRect);
    g.target->DrawBitmap(_p2StatsPanel->Image(), p2StatsPanelRect);
    if (_uiState.currentPlayer == 0)
        g.target->DrawBitmap(_p1ExtraActionsLabel->Image(), p1ActionsLabelRect);
    else
        g.target->DrawBitmap(_p2ExtraActionsLabel->Image(), p2ActionsLabelRect);
}

void zcom::Board::_OnResize(int width, int height)
{
    _CalculateCardTargetPositions();
}

zcom::EventTargets zcom::Board::_OnMouseMove(int deltaX, int deltaY)
{
    if (!_heldCard)
    {
        _DetectHoveredCard();
    }
    return EventTargets().Add(this, GetMousePosX(), GetMousePosY());
}

void zcom::Board::_OnMouseEnter()
{

}

void zcom::Board::_OnMouseLeave()
{
    _hoveredCard = nullptr;
    _hoveredCardInHand = false;
}

zcom::EventTargets zcom::Board::_OnLeftPressed(int x, int y)
{
    if (_hoveredCard && !_noClickMode)
    {
        _Card* card = _GetCardFromPointer(_hoveredCard);

        // In draw mode only allow deck cards
        if (_drawCardMode)
        {
            if (card->set.set != cards::CardSets::DECK)
                return EventTargets().Add(this, x, y);
            if (!_allowedCardTypesToDraw.empty() && !zutil::Contains(_allowedCardTypesToDraw, card->card->GetCardType()))
                return EventTargets().Add(this, x, y);
        }

        // Special logic for card select modes
        if (_chooseCardsFromHandMode)
        {
            if (_chosenCardsFromHand.find(_hoveredCard) == _chosenCardsFromHand.end())
            {
                if (_chosenCardsFromHand.size() < _chooseCardsFromHandMaxCardCount)
                {
                    _chosenCardsFromHand.insert(_hoveredCard);
                    _selectedCardInHandHandler(_hoveredCard);
                }
            }
            else
            {
                _chosenCardsFromHand.erase(_hoveredCard);
                _deselectedCardInHandHandler(_hoveredCard);
            }

            return EventTargets().Add(this, x, y);
        }
        else if (_chooseCardsFromActiveCardsMode)
        {
            if (_chosenCardsFromActiveCards.find(_hoveredCard) == _chosenCardsFromActiveCards.end())
            {
                if (_chosenCardsFromActiveCards.size() < _chooseCardsFromActiveCardsMaxCardCount)
                {
                    _chosenCardsFromActiveCards.insert(_hoveredCard);
                    _selectedCardInActiveCardsHandler(_hoveredCard);
                }
            }
            else
            {
                _chosenCardsFromActiveCards.erase(_hoveredCard);
                _deselectedCardInActiveCardsHandler(_hoveredCard);
            }

            return EventTargets().Add(this, x, y);
        }

        _heldCard = _hoveredCard;
        _hoveredCard = nullptr;
        _hoveredCardInHand = false;
    }
    return EventTargets().Add(this, x, y);
}

zcom::EventTargets zcom::Board::_OnLeftReleased(int x, int y)
{
    //if (_)

    if (_heldCard)
    {
        _Card* card = _GetCardFromPointer(_heldCard);

        float viewWidth = GetWidth();
        float viewHeight = GetHeight();

        bool returnToStartPos = true;
        if (card->set.set == cards::CardSets::DECK)
        {
            // Draw
            if (GetMousePosX() > 500.0f)
            {
                card->xPos = GetMousePosX();
                card->yPos = GetMousePosY();
                cards::Card* drawnCard = _core->DrawCard(_heldCard->GetCardType(), _uiState.currentPlayer, !_drawCardMode);
                returnToStartPos = false;

                if (_drawCardMode)
                    _drawCardHandler(drawnCard);
            }
        }
        else if (card->set.set == cards::CardSets::HAND && card->set.playerIndex == _uiState.currentPlayer)
        {
            // Discard
            if (GetMousePosX() > viewWidth - 500.0f)
            {
                card->xPos = GetMousePosX();
                card->yPos = GetMousePosY();
                _core->DiscardCard(_heldCard, _uiState.currentPlayer);
                returnToStartPos = false;
            }
            // Play
            else if (GetMousePosX() > 500.0f && GetMousePosY() < viewHeight - 300.0f)
            {
                card->xPos = GetMousePosX();
                card->yPos = GetMousePosY();
                cards::PlayResult result = _core->PlayCard(_heldCard);
                if (result.waitForInput)
                    _HandleInputRequest(result);
                returnToStartPos = false;
            }
        }

        if (returnToStartPos)
        {
            card->startXPos = GetMousePosX();
            card->startYPos = GetMousePosY();
            card->startRotation = 0.0f;
            card->targetXPos = card->xPos;
            card->targetYPos = card->yPos;
            card->targetRotation = card->rotation;
            card->moving = true;
            card->moveStartTime = ztime::Main();
            card->moveDuration = Duration(300, MILLISECONDS);
        }

        _heldCard = nullptr;
        _DetectHoveredCard();
    }
    return EventTargets().Add(this, x, y);
}

zcom::EventTargets zcom::Board::_OnRightPressed(int x, int y)
{
    DamageProperties props;
    props.target = 0;
    props.amount = 1;
    _core->Damage(props);
    return EventTargets().Add(this, x, y);
}

void zcom::Board::_DetectHoveredCard()
{
    _hoveredCard = _GetHoveredCard();
    _hoveredCardInHand = false;
    if (_hoveredCard)
    {
        _Card* card = _GetCardFromPointer(_hoveredCard);
        if (card->set.set == cards::CardSets::HAND)
            _hoveredCardInHand = true;
    }
}

void zcom::Board::_HandleInputRequest(cards::PlayResult& result)
{
    if (result.inputRequest.inputType == UserInputType::RESUME)
    {
        UserInputResponse response;
        response.inputParams = std::move(result.inputRequest.inputParams);
        auto resumeResult = _core->ResumePlay(std::move(response));

        if (resumeResult.waitForInput)
            _HandleInputRequest(resumeResult);
    }
    else
    {
        _pendingUserInputRequest = std::move(result.inputRequest);
        _uiEventQueue.push_back(UIEvent{ UIEvent::USER_INPUT_REQUEST, &_pendingUserInputRequest });
    }
}

UserInputRequest* zcom::Board::GetUserInputRequest()
{
    if (_userInputRequestAvailable)
        return &_userInputRequest;
    else
        return nullptr;
}

void zcom::Board::SendUserInputResponse()
{
    if (!_userInputRequestAvailable)
        return;

    _userInputRequestAvailable = false;
    UserInputResponse response;
    response.inputParams = std::move(_userInputRequest.inputParams);
    auto resumeResult = _core->ResumePlay(std::move(response));

    if (resumeResult.waitForInput)
        _HandleInputRequest(resumeResult);
}

void zcom::Board::_GenerateCardBitmap(Graphics g, ID2D1Bitmap** bitmapRef, D2D1_COLOR_F color)
{
    ID2D1Image* stash = nullptr;

    // Create bitmap of card
    ID2D1Bitmap1* contentBitmap = nullptr;
    g.target->CreateBitmap(
        D2D1::SizeU(RENDER_CARD_WIDTH, RENDER_CARD_HEIGHT),
        nullptr,
        0,
        D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET,
            { DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED }
        ),
        &contentBitmap
    );

    ID2D1SolidColorBrush* fillBrush = nullptr;
    g.target->CreateSolidColorBrush(color, &fillBrush);
    ID2D1SolidColorBrush* borderBrush = nullptr;
    color.r *= 0.5f;
    color.g *= 0.5f;
    color.b *= 0.5f;
    color.a = 0.8f;
    g.target->CreateSolidColorBrush(color, &borderBrush);

    D2D1_RECT_F cardRect = D2D1::RectF(
        2.0f,
        2.0f,
        RENDER_CARD_WIDTH - 2.0f,
        RENDER_CARD_HEIGHT - 2.0f
    );
    D2D1_ROUNDED_RECT roundedCardRect = D2D1::RoundedRect(cardRect, 5.0f, 5.0f);

    D2D1_RECT_F borderRect = D2D1::RectF(
        1.0f,
        1.0f,
        RENDER_CARD_WIDTH - 1.0f,
        RENDER_CARD_HEIGHT - 1.0f
    );
    D2D1_ROUNDED_RECT roundedBorderRect = D2D1::RoundedRect(borderRect, 5.0f, 5.0f);

    g.target->GetTarget(&stash);
    g.target->SetTarget(contentBitmap);
    g.target->Clear();
    g.target->FillRoundedRectangle(roundedCardRect, fillBrush);
    g.target->DrawRoundedRectangle(roundedBorderRect, borderBrush, 2.0f);
    g.target->SetTarget(stash);
    stash->Release();

    // Create final bitmap
    g.target->CreateBitmap(
        D2D1::SizeU(RENDER_CARD_WIDTH, RENDER_CARD_HEIGHT),
        nullptr,
        0,
        D2D1::BitmapProperties({ DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED }),
        bitmapRef
    );

    {
        D2D1_POINT_2U point = D2D1::Point2U();
        D2D1_RECT_U rect = D2D1::RectU(0, 0, RENDER_CARD_WIDTH, RENDER_CARD_HEIGHT);
        //HRESULT hr = (*bitmapRef)->CopyFromBitmap(&point, contentBitmap, &rect);
        g.target->GetTarget(&stash);
        //g.target->SetTarget(shadowBitmap);
        g.target->SetTarget(contentBitmap);
        (*bitmapRef)->CopyFromRenderTarget(&point, g.target, &rect);
        g.target->SetTarget(stash);
        stash->Release();
    }

    fillBrush->Release();
    borderBrush->Release();
    //shadowEffect->Release();
    //shadowBitmap->Release();
    contentBitmap->Release();
}

void zcom::Board::_UpdateStatLabels()
{
    { // Update player 1 stat labels
        auto& player = _uiState.players[0];

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
        auto& player = _uiState.players[1];

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

void zcom::Board::_CreateMissingCards()
{
    for (auto& cardPtr : _core->GetState().offenseDeck)
    {
        auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
        if (it == _cards.end())
            _cards.push_back(_Card{ cardPtr.get() });
    }
    for (auto& cardPtr : _core->GetState().defenseDeck)
    {
        auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
        if (it == _cards.end())
            _cards.push_back(_Card{ cardPtr.get() });
    }
    for (auto& cardPtr : _core->GetState().utilityDeck)
    {
        auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
        if (it == _cards.end())
            _cards.push_back(_Card{ cardPtr.get() });
    }
    for (auto& cardPtr : _core->GetState().comboDeck)
    {
        auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
        if (it == _cards.end())
            _cards.push_back(_Card{ cardPtr.get() });
    }
    for (auto& player : _core->GetState().players)
    {
        for (auto& cardPtr : player.hand)
        {
            auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
            if (it == _cards.end())
                _cards.push_back(_Card{ cardPtr.get() });
        }
        for (auto& cardPtr : player.activeCards)
        {
            auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
            if (it == _cards.end())
                _cards.push_back(_Card{ cardPtr.get() });
        }
    }
    for (auto& cardPtr : _core->GetState().graveyard)
    {
        auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
        if (it == _cards.end())
            _cards.push_back(_Card{ cardPtr.get() });
    }
    for (auto& cardPtr : _core->GetState().inPlayCards)
    {
        auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
        if (it == _cards.end())
            _cards.push_back(_Card{ cardPtr.get() });
    }
    for (auto& cardPtr : _core->GetState().destroyedCards)
    {
        auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
        if (it == _cards.end())
            _cards.push_back(_Card{ cardPtr.get() });
    }
}

void zcom::Board::_SyncCardPositions()
{
    for (auto& card : _cards)
        card.set = _core->GetCardSet(card.card);
}

void zcom::Board::_SyncUISets()
{
    for (int i = 0; i < 2; i++)
    {
        _uiState.players[i].hand.clear();
        _uiState.players[i].activeCards.clear();

        for (auto& card : _core->GetState().players[i].hand)
            _uiState.players[i].hand.push_back(card.get());
        for (auto& card : _core->GetState().players[i].activeCards)
            _uiState.players[i].activeCards.push_back(card.get());
    }

    _uiState.offenseDeck.clear();
    _uiState.defenseDeck.clear();
    _uiState.utilityDeck.clear();
    _uiState.comboDeck.clear();
    _uiState.graveyard.clear();
    _uiState.inPlayCards.clear();
    _uiState.destroyedCards.clear();

    for (auto& card : _core->GetState().offenseDeck)
        _uiState.offenseDeck.push_back(card.get());
    for (auto& card : _core->GetState().defenseDeck)
        _uiState.defenseDeck.push_back(card.get());
    for (auto& card : _core->GetState().utilityDeck)
        _uiState.utilityDeck.push_back(card.get());
    for (auto& card : _core->GetState().comboDeck)
        _uiState.comboDeck.push_back(card.get());
    for (auto& card : _core->GetState().graveyard)
        _uiState.graveyard.push_back(card.get());
    for (auto& card : _core->GetState().inPlayCards)
        _uiState.inPlayCards.push_back(card.get());
    for (auto& card : _core->GetState().destroyedCards)
        _uiState.destroyedCards.push_back(card.get());
}

void zcom::Board::_CalculateCardTargetPositions()
{
    float viewWidth = GetWidth();
    float viewHeight = GetHeight();

    float gapFromScreenEdge = 100.0f;
    float gapBetweenDecks = 30.0f;

    for (auto& card : _cards)
    {
        if (card.card == _hoveredCard)
            card.scale = 1.1f;
        else
            card.scale = 1.0f;



        float offenseDeckCenterPosX = gapFromScreenEdge + CARD_HEIGHT / 2;
        float offenseDeckCenterPosY = viewHeight / 2 - gapBetweenDecks * 1.5f - CARD_WIDTH * 1.5f;
        float defenseDeckCenterPosX = gapFromScreenEdge + CARD_HEIGHT / 2;
        float defenseDeckCenterPosY = viewHeight / 2 - gapBetweenDecks * 0.5f - CARD_WIDTH * 0.5f;
        float utilityDeckCenterPosX = gapFromScreenEdge + CARD_HEIGHT / 2;
        float utilityDeckCenterPosY = viewHeight / 2 + gapBetweenDecks * 0.5f + CARD_WIDTH * 0.5f;
        float comboDeckCenterPosX = gapFromScreenEdge + CARD_HEIGHT / 2;
        float comboDeckCenterPosY = viewHeight / 2 + gapBetweenDecks * 1.5f + CARD_WIDTH * 1.5f;

        if (card.set.set == cards::CardSets::DECK)
        {
            if (card.card->GetCardType() == cards::CardType::OFFENSE)
            {
                for (int i = 0; i < _uiState.offenseDeck.size(); i++)
                {
                    if (card.card == _uiState.offenseDeck[i])
                    {
                        card.zIndex = i;
                        card.targetXPos = offenseDeckCenterPosX - i * 1.25f;
                        card.targetYPos = offenseDeckCenterPosY;
                        card.targetRotation = PI / 2;
                        card.targetOpacity = 1.0f;
                        break;
                    }
                }
            }
            else if (card.card->GetCardType() == cards::CardType::DEFENSE)
            {
                for (int i = 0; i < _uiState.defenseDeck.size(); i++)
                {
                    if (card.card == _uiState.defenseDeck[i])
                    {
                        card.zIndex = i;
                        card.targetXPos = defenseDeckCenterPosX - i * 1.25f;
                        card.targetYPos = defenseDeckCenterPosY;
                        card.targetRotation = PI / 2;
                        card.targetOpacity = 1.0f;
                        break;
                    }
                }
            }
            else if (card.card->GetCardType() == cards::CardType::UTILITY)
            {
                for (int i = 0; i < _uiState.utilityDeck.size(); i++)
                {
                    if (card.card == _uiState.utilityDeck[i])
                    {
                        card.zIndex = i;
                        card.targetXPos = utilityDeckCenterPosX - i * 1.25f;
                        card.targetYPos = utilityDeckCenterPosY;
                        card.targetRotation = PI / 2;
                        card.targetOpacity = 1.0f;
                        break;
                    }
                }
            }
            else if (card.card->GetCardType() == cards::CardType::COMBO)
            {
                for (int i = 0; i < _uiState.comboDeck.size(); i++)
                {
                    if (card.card == _uiState.comboDeck[i])
                    {
                        card.zIndex = i;
                        card.targetXPos = comboDeckCenterPosX - i * 1.25f;
                        card.targetYPos = comboDeckCenterPosY;
                        card.targetRotation = PI / 2;
                        card.targetOpacity = 1.0f;
                        break;
                    }
                }
            }
        }
        else if (card.set.set == cards::CardSets::HAND)
        {
            if (card.set.playerIndex == _uiState.currentPlayer)
            {
                int cardsInHand = _uiState.players[_uiState.currentPlayer].hand.size();
                float circleCenterOffset = 800.0f + cardsInHand * 100.0f;
                Pos2D<float> circleCenter = { viewWidth / 2, viewHeight + circleCenterOffset };
                Pos2D<float> circleTop = circleCenter + Pos2D<float>(0.0f, -(circleCenterOffset + 100.0f));
                float gapBetweenCards = 0.18f;
                if (cardsInHand > 0)
                    gapBetweenCards -= 0.18f * std::powf(1.0f - 1.0f / cardsInHand, 2.0f);

                for (int i = 0; i < cardsInHand; i++)
                {
                    if (card.card == _uiState.players[_uiState.currentPlayer].hand[i])
                    {
                        float cardAngle = (gapBetweenCards * (cardsInHand - 1)) / 2 - gapBetweenCards * i;
                        Pos2D<float> cardPos = point_rotated_by(circleCenter, circleTop, cardAngle);

                        card.zIndex = i;
                        card.targetXPos = cardPos.x;
                        card.targetYPos = cardPos.y;
                        card.targetRotation = cardAngle * (0.0f + 0.2f * cardsInHand);
                        card.targetOpacity = 1.0f;
                        break;
                    }
                }
            }
            else
            {
                int cardsInHand = _uiState.players[_uiState.opposingPlayer].hand.size();
                float circleCenterOffset = 800.0f + cardsInHand * 100.0f;
                Pos2D<float> circleCenter = { viewWidth / 2, -circleCenterOffset };
                Pos2D<float> circleBottom = circleCenter + Pos2D<float>(0.0f, circleCenterOffset + 100.0f);
                float gapBetweenCards = 0.18f;
                if (cardsInHand > 0)
                    gapBetweenCards -= 0.18f * std::powf(1.0f - 1.0f / cardsInHand, 2.0f);
                float startAngle = PI;

                for (int i = 0; i < cardsInHand; i++)
                {
                    if (card.card == _uiState.players[_uiState.opposingPlayer].hand[i])
                    {
                        float cardAngle = (gapBetweenCards * (cardsInHand - 1)) / 2 - gapBetweenCards * i;
                        Pos2D<float> cardPos = point_rotated_by(circleCenter, circleBottom, cardAngle);

                        card.zIndex = i;
                        card.targetXPos = cardPos.x;
                        card.targetYPos = cardPos.y;
                        card.targetRotation = startAngle + cardAngle * (0.0f + 0.2f * cardsInHand);
                        card.targetOpacity = 1.0f;
                        break;
                    }
                }
            }
        }
        else if (card.set.set == cards::CardSets::ACTIVE_CARDS)
        {
            float gapBetweenCards = 30.0f;
            float horizontalGapFromEdge = 500.0f;
            float maxActiveCardWidth = viewWidth - horizontalGapFromEdge * 2;

            if (card.set.playerIndex == _uiState.currentPlayer)
            {
                for (int i = 0; i < _uiState.players[_uiState.currentPlayer].activeCards.size(); i++)
                {
                    if (card.card == _uiState.players[_uiState.currentPlayer].activeCards[i])
                    {
                        card.zIndex = -100 + i;
                        card.targetXPos = horizontalGapFromEdge + CARD_WIDTH / 2 + (CARD_WIDTH + gapBetweenCards) * i;
                        card.targetYPos = viewHeight - 250.0f - CARD_HEIGHT / 2;
                        card.targetRotation = 0.0f;
                        card.targetOpacity = 1.0f;
                        break;
                    }
                }
            }
            else
            {
                for (int i = 0; i < _uiState.players[_uiState.opposingPlayer].activeCards.size(); i++)
                {
                    if (card.card == _uiState.players[_uiState.opposingPlayer].activeCards[i])
                    {
                        card.zIndex = -100 + i;
                        card.targetXPos = viewWidth - horizontalGapFromEdge - CARD_WIDTH / 2 - (CARD_WIDTH + gapBetweenCards) * i;
                        card.targetYPos = 250.0f + CARD_HEIGHT / 2;
                        card.targetRotation = PI;
                        card.targetOpacity = 1.0f;
                        break;
                    }
                }
            }
        }
        else if (card.set.set == cards::CardSets::GRAVEYARD)
        {
            float graveyardDeckCenterPosX = viewWidth - gapFromScreenEdge - CARD_WIDTH / 2;
            float graveyardDeckCenterPosY = viewHeight / 2;

            for (int i = 0; i < _uiState.graveyard.size(); i++)
            {
                if (card.card == _uiState.graveyard[i])
                {
                    card.zIndex = i;
                    card.targetXPos = graveyardDeckCenterPosX + i * 0.75f;
                    card.targetYPos = graveyardDeckCenterPosY;
                    card.targetRotation = 0.0f;
                    card.targetOpacity = 1.0f;
                    break;
                }
            }
        }
        else if (card.set.set == cards::CardSets::DESTROYED)
        {
            for (int i = 0; i < _uiState.destroyedCards.size(); i++)
            {
                if (card.card == _uiState.destroyedCards[i])
                {
                    card.targetOpacity = 0.0f;
                    break;
                }
            }
        }
        else if (card.set.set == cards::CardSets::IN_PLAY)
        {
            int cardsInPlay = _uiState.inPlayCards.size();
            float gapBetweenCards = 30.0f;
            float layoutWidth = (cardsInPlay - 1) * (CARD_WIDTH + gapBetweenCards);

            for (int i = 0; i < cardsInPlay; i++)
            {
                if (card.card == _uiState.inPlayCards[i])
                {
                    card.zIndex = i + 100;
                    card.targetXPos = viewWidth / 2 - layoutWidth / 2 + (CARD_WIDTH + gapBetweenCards) * i;
                    card.targetYPos = viewHeight / 2;
                    card.targetRotation = 0.0f;
                    card.targetOpacity = 1.0f;
                    break;
                }
            }
        }
    }
}

cards::Card* zcom::Board::_GetHoveredCard()
{
    if (!GetMouseInside())
        return nullptr;

    std::vector<_Card> sortedCards = _cards;
    std::sort(sortedCards.begin(), sortedCards.end(), [&](const _Card& card1, const _Card& card2) { return card1.zIndex > card2.zIndex; });
    // Move hovered card to beggining
    auto it = std::find_if(sortedCards.begin(), sortedCards.end(), [&](const _Card& card) { return card.card == _hoveredCard && !_hoveredCardInHand; });
    if (it != sortedCards.end())
    {
        _Card card = *it;
        sortedCards.erase(it);
        sortedCards.insert(sortedCards.begin(), card);
    }

    for (auto& card : sortedCards)
    {
        // Skip moving cards
        if (card.moving)
            continue;

        // Skip cards not on top of stacked decks
        if (card.set.set == cards::CardSets::DECK)
        {
            if (card.card != _uiState.GetDeck(card.card->GetCardType()).back())
                continue;
        }
        else if (card.set.set == cards::CardSets::GRAVEYARD)
        {
            if (card.card != _uiState.graveyard.back())
                continue;
        }

        if (_chooseCardsFromHandMode)
        {
            if (_uiState.currentPlayer != _chooseCardsFromHandChoosingPlayerIndex)
                continue;
            if (card.set.set != cards::CardSets::HAND || card.set.playerIndex != _chooseCardsFromHandTargetPlayerIndex)
                continue;
            if (!_chooseCardsFromHandAllowedTypes.empty() && !zutil::Contains(_chooseCardsFromHandAllowedTypes, card.card->GetCardType()))
                continue;
        }
        if (_chooseCardsFromActiveCardsMode)
        {
            if (_uiState.currentPlayer != _chooseCardsFromActiveCardsChoosingPlayerIndex)
                continue;
            if (card.set.set != cards::CardSets::ACTIVE_CARDS || card.set.playerIndex != _chooseCardsFromActiveCardsTargetPlayerIndex)
                continue;
            if (!_chooseCardsFromActiveCardsAllowedTypes.empty() && !zutil::Contains(_chooseCardsFromActiveCardsAllowedTypes, card.card->GetCardType()))
                continue;
        }

        Pos2D<float> mousePos = { (float)GetMousePosX(), (float)GetMousePosY() };
        Pos2D<float> cardCenterPos = { card.xPos, card.yPos };
        // Rotate mouse position around card center to align relative coordinates to upright card position
        Pos2D<float> adjustedMousePos = point_rotated_by(cardCenterPos, mousePos, -card.targetRotation);

        float cardLeftSide = card.xPos - (CARD_WIDTH / 2) * card.scale;
        float cardRightSide = card.xPos + (CARD_WIDTH / 2) * card.scale;
        float cardTopSide = card.yPos - (CARD_HEIGHT / 2) * card.scale;
        float cardBottomSide = card.yPos + (CARD_HEIGHT / 2) * card.scale;
        if (adjustedMousePos.x >= cardLeftSide && adjustedMousePos.x <= cardRightSide &&
            adjustedMousePos.y >= cardTopSide && adjustedMousePos.y <= cardBottomSide)
        {
            return card.card;
        }
    }

    return nullptr;
}

zcom::Board::_Card* zcom::Board::_GetCardFromPointer(cards::Card* cardPtr)
{
    for (auto& card : _cards)
    {
        if (card.card == cardPtr)
        {
            return &card;
        }
    }
    return nullptr;
}

EventTimings GetEventTimings(UIEvent::Type event)
{
    switch (event)
    {
    case UIEvent::USER_INPUT_REQUEST:           return { Duration(0, MILLISECONDS), Duration(0, MILLISECONDS) };
    case UIEvent::TURN_BEGIN:                   return { Duration(500, MILLISECONDS), Duration(500, MILLISECONDS) };
    case UIEvent::TURN_END:                     return { Duration(500, MILLISECONDS), Duration(500, MILLISECONDS) };
    case UIEvent::ACTION_COUNT_CHANGED:         return { Duration(0, MILLISECONDS), Duration(0, MILLISECONDS) };
    case UIEvent::POST_DAMAGE:                  return { Duration(500, MILLISECONDS), Duration(500, MILLISECONDS) };
    case UIEvent::POST_HEAL:                    return { Duration(500, MILLISECONDS), Duration(500, MILLISECONDS) };
    case UIEvent::POST_ADD_ARMOR:               return { Duration(500, MILLISECONDS), Duration(500, MILLISECONDS) };
    case UIEvent::POST_DESTROY_ARMOR:           return { Duration(500, MILLISECONDS), Duration(500, MILLISECONDS) };
    case UIEvent::POST_HEALTH_CHANGE:           return { Duration(500, MILLISECONDS), Duration(500, MILLISECONDS) };
    case UIEvent::POST_MAX_HEALTH_CHANGE:       return { Duration(500, MILLISECONDS), Duration(500, MILLISECONDS) };
    case UIEvent::POST_ARMOR_CHANGE:            return { Duration(500, MILLISECONDS), Duration(500, MILLISECONDS) };
    case UIEvent::CARD_ENTER_HAND:              return { Duration(100, MILLISECONDS), Duration(100, MILLISECONDS) };
    case UIEvent::CARD_LEAVE_HAND:              return { Duration(0, MILLISECONDS), Duration(100, MILLISECONDS) };
    case UIEvent::CARD_ENTER_ACTIVES:           return { Duration(100, MILLISECONDS), Duration(100, MILLISECONDS) };
    case UIEvent::CARD_LEAVE_ACTIVES:           return { Duration(0, MILLISECONDS), Duration(100, MILLISECONDS) };
    case UIEvent::CARD_ENTER_DECK:              return { Duration(100, MILLISECONDS), Duration(100, MILLISECONDS) };
    case UIEvent::CARD_LEAVE_DECK:              return { Duration(0, MILLISECONDS), Duration(100, MILLISECONDS) };
    case UIEvent::CARD_ENTER_GRAVEYARD:         return { Duration(100, MILLISECONDS), Duration(100, MILLISECONDS) };
    case UIEvent::CARD_LEAVE_GRAVEYARD:         return { Duration(0, MILLISECONDS), Duration(100, MILLISECONDS) };
    case UIEvent::CARD_ENTER_DESTROYED_CARDS:   return { Duration(100, MILLISECONDS), Duration(100, MILLISECONDS) };
    case UIEvent::CARD_LEAVE_DESTROYED_CARDS:   return { Duration(0, MILLISECONDS), Duration(100, MILLISECONDS) };
    case UIEvent::CARD_ENTER_IN_PLAY_CARDS:     return { Duration(100, MILLISECONDS), Duration(100, MILLISECONDS) };
    case UIEvent::CARD_LEAVE_IN_PLAY_CARDS:     return { Duration(0, MILLISECONDS), Duration(100, MILLISECONDS) };
    case UIEvent::CARD_ENTER_DISPLAYED_CARDS:   return { Duration(100, MILLISECONDS), Duration(100, MILLISECONDS) };
    case UIEvent::CARD_LEAVE_DISPLAYED_CARDS:   return { Duration(0, MILLISECONDS), Duration(100, MILLISECONDS) };
    case UIEvent::HAND_REVEAL_STATE_CHANGED:    return { Duration(500, MILLISECONDS), Duration(500, MILLISECONDS) };
    }
}

void zcom::Board::_HandleEvents()
{
    while (!_uiEventQueue.empty())
    {
        if (ztime::Main() < _prevEventTime + _minTimeUntilNextEvent)
            return;

        UIEvent event = _uiEventQueue.front();
        if (ztime::Main() < _prevEventTime + GetEventTimings(event.type).preEvent)
            return;
        _uiEventQueue.erase(_uiEventQueue.begin());

        switch (event.type)
        {
        case UIEvent::USER_INPUT_REQUEST:
        {
            _userInputRequest = std::move(_pendingUserInputRequest);
            _userInputRequestAvailable = true;
            break;
        }
        case UIEvent::TURN_BEGIN:
        {
            TurnBeginEvent turnBeginEvent = std::any_cast<TurnBeginEvent>(event.event);
            _uiState.currentPlayer = turnBeginEvent.playerIndex;
            _uiState.opposingPlayer = turnBeginEvent.opponentIndex;

            _CalculateCardTargetPositions();
            for (auto& card : _cards)
            {
                card.startXPos = card.xPos;
                card.startYPos = card.yPos;
                card.startRotation = card.rotation;
                card.moving = true;
                card.moveStartTime = ztime::Main();
                card.moveDuration = Duration(600, MILLISECONDS);
            }

            break;
        }
        case UIEvent::ACTION_COUNT_CHANGED:
        {
            _uiState.players[0].actionsLeft = _core->GetState().players[0].actionsLeft;
            _uiState.players[0].extraActions = _core->GetState().players[0].extraActions;
            _uiState.players[1].actionsLeft = _core->GetState().players[1].actionsLeft;
            _uiState.players[1].extraActions = _core->GetState().players[1].extraActions;
            _UpdateStatLabels();
            break;
        }
        case UIEvent::POST_DAMAGE:
        {
            //_uiState.players[0].health = _core->GetState().players[0].health;
            //_uiState.players[1].health = _core->GetState().players[1].health;
            //_uiState.players[0].armor = _core->GetState().players[0].armor;
            //_uiState.players[1].armor = _core->GetState().players[1].armor;
            //_UpdateStatLabels();
            break;
        }
        case UIEvent::POST_HEAL:
        {
            //_uiState.players[0].health = _core->GetState().players[0].health;
            //_uiState.players[1].health = _core->GetState().players[1].health;
            //_UpdateStatLabels();
            break;
        }
        case UIEvent::POST_ADD_ARMOR:
        {
            //_uiState.players[0].armor = _core->GetState().players[0].armor;
            //_uiState.players[1].armor = _core->GetState().players[1].armor;
            //_UpdateStatLabels();
            break;
        }
        case UIEvent::POST_DESTROY_ARMOR:
        {
            //_uiState.players[0].armor = _core->GetState().players[0].armor;
            //_uiState.players[1].armor = _core->GetState().players[1].armor;
            //_UpdateStatLabels();
            break;
        }
        case UIEvent::POST_HEALTH_CHANGE:
        {
            _uiState.players[0].health = _core->GetState().players[0].health;
            _uiState.players[1].health = _core->GetState().players[1].health;
            _UpdateStatLabels();
            break;
        }
        case UIEvent::POST_MAX_HEALTH_CHANGE:
        {
            _uiState.players[0].maxHealth = _core->GetState().players[0].maxHealth;
            _uiState.players[1].maxHealth = _core->GetState().players[1].maxHealth;
            _UpdateStatLabels();
            break;
        }
        case UIEvent::POST_ARMOR_CHANGE:
        {
            _uiState.players[0].armor = _core->GetState().players[0].armor;
            _uiState.players[1].armor = _core->GetState().players[1].armor;
            _UpdateStatLabels();
            break;
        }
        case UIEvent::CARD_LEAVE_HAND:
        {
            UIEvent pairedEvent = _uiEventQueue.front();
            _uiEventQueue.erase(_uiEventQueue.begin());

            CardLeaveHandEvent cardLeaveHandEvent = std::any_cast<CardLeaveHandEvent>(event.event);
            _Card* card = _GetCardFromPointer(cardLeaveHandEvent.card);
            if (!card)
                break;

            _uiState.RemoveCardFromHand(card->card, cardLeaveHandEvent.playerIndex);
            _ProcessCardMove(card, pairedEvent);
            break;
        }
        case UIEvent::CARD_LEAVE_ACTIVES:
        {
            UIEvent pairedEvent = _uiEventQueue.front();
            _uiEventQueue.erase(_uiEventQueue.begin());

            CardLeaveActivesEvent cardLeaveActivesEvent = std::any_cast<CardLeaveActivesEvent>(event.event);
            _Card* card = _GetCardFromPointer(cardLeaveActivesEvent.card);
            if (!card)
                break;

            _uiState.RemoveCardFromHand(card->card, cardLeaveActivesEvent.playerIndex);
            _ProcessCardMove(card, pairedEvent);
            break;
        }
        case UIEvent::CARD_LEAVE_DECK:
        {
            UIEvent pairedEvent = _uiEventQueue.front();
            _uiEventQueue.erase(_uiEventQueue.begin());

            CardLeaveDeckEvent cardLeaveDeckEvent = std::any_cast<CardLeaveDeckEvent>(event.event);
            _Card* card = _GetCardFromPointer(cardLeaveDeckEvent.card);
            if (!card)
                break;

            _uiState.RemoveCardFromDeck(card->card);
            _ProcessCardMove(card, pairedEvent);
            break;
        }
        case UIEvent::CARD_LEAVE_GRAVEYARD:
        {
            UIEvent pairedEvent = _uiEventQueue.front();
            _uiEventQueue.erase(_uiEventQueue.begin());

            CardLeaveGraveyardEvent cardLeaveGraveyardEvent = std::any_cast<CardLeaveGraveyardEvent>(event.event);
            _Card* card = _GetCardFromPointer(cardLeaveGraveyardEvent.card);
            if (!card)
                break;

            _uiState.RemoveCardFromGraveyard(card->card);
            _ProcessCardMove(card, pairedEvent);
            break;
        }
        case UIEvent::CARD_LEAVE_DESTROYED_CARDS:
        {
            UIEvent pairedEvent = _uiEventQueue.front();
            _uiEventQueue.erase(_uiEventQueue.begin());

            CardLeaveDestroyedCardsEvent cardLeaveDestroyedCardsEvent = std::any_cast<CardLeaveDestroyedCardsEvent>(event.event);
            _Card* card = _GetCardFromPointer(cardLeaveDestroyedCardsEvent.card);
            if (!card)
                break;

            _uiState.RemoveCardFromDestroyedCards(card->card);
            _ProcessCardMove(card, pairedEvent);
            break;
        }
        case UIEvent::CARD_LEAVE_IN_PLAY_CARDS:
        {
            UIEvent pairedEvent = _uiEventQueue.front();
            _uiEventQueue.erase(_uiEventQueue.begin());

            CardLeaveInPlayCardsEvent cardLeaveInPlayCardsEvent = std::any_cast<CardLeaveInPlayCardsEvent>(event.event);
            _Card* card = _GetCardFromPointer(cardLeaveInPlayCardsEvent.card);
            if (!card)
                break;

            _uiState.RemoveCardFromInPlayCards(card->card);
            _ProcessCardMove(card, pairedEvent);
            break;
        }
        case UIEvent::CARD_ENTER_HAND:
        {
            _CreateMissingCards();

            CardEnterHandEvent cardEnterHandEvent = std::any_cast<CardEnterHandEvent>(event.event);
            _Card* card = _GetCardFromPointer(cardEnterHandEvent.card);
            if (!card)
                break;

            _uiState.AddCardToHand(card->card, cardEnterHandEvent.playerIndex);
            _MoveNewCardToSet(card, { cards::CardSets::HAND, cardEnterHandEvent.playerIndex });
            break;
        }
        case UIEvent::CARD_ENTER_ACTIVES:
        {
            _CreateMissingCards();

            CardEnterActivesEvent cardEnterActivesEvent = std::any_cast<CardEnterActivesEvent>(event.event);
            _Card* card = _GetCardFromPointer(cardEnterActivesEvent.card);
            if (!card)
                break;

            _uiState.AddCardToActiveCards(card->card, cardEnterActivesEvent.playerIndex);
            _MoveNewCardToSet(card, { cards::CardSets::ACTIVE_CARDS, cardEnterActivesEvent.playerIndex });
            break;
        }
        case UIEvent::CARD_ENTER_DECK:
        {
            _CreateMissingCards();

            CardEnterDeckEvent cardEnterDeckEvent = std::any_cast<CardEnterDeckEvent>(event.event);
            _Card* card = _GetCardFromPointer(cardEnterDeckEvent.card);
            if (!card)
                break;

            _uiState.AddCardToDeck(card->card);
            _MoveNewCardToSet(card, { cards::CardSets::DECK, -1 });
            break;
        }
        case UIEvent::CARD_ENTER_GRAVEYARD:
        {
            _CreateMissingCards();

            CardEnterGraveyardEvent cardEnterGraveyardEvent = std::any_cast<CardEnterGraveyardEvent>(event.event);
            _Card* card = _GetCardFromPointer(cardEnterGraveyardEvent.card);
            if (!card)
                break;

            _uiState.AddCardToGraveyard(card->card);
            _MoveNewCardToSet(card, { cards::CardSets::GRAVEYARD, -1 });
            break;
        }
        case UIEvent::CARD_ENTER_DESTROYED_CARDS:
        {
            _CreateMissingCards();

            CardEnterDestroyedCardsEvent cardEnterDestroyedCardsEvent = std::any_cast<CardEnterDestroyedCardsEvent>(event.event);
            _Card* card = _GetCardFromPointer(cardEnterDestroyedCardsEvent.card);
            if (!card)
                break;

            _uiState.AddCardToDestroyedCards(card->card);
            _MoveNewCardToSet(card, { cards::CardSets::DESTROYED, -1 });
            break;
        }
        case UIEvent::CARD_ENTER_IN_PLAY_CARDS:
        {
            _CreateMissingCards();

            CardEnterInPlayCardsEvent cardEnterInPlayCardsEvent = std::any_cast<CardEnterInPlayCardsEvent>(event.event);
            _Card* card = _GetCardFromPointer(cardEnterInPlayCardsEvent.card);
            if (!card)
                break;

            _uiState.AddCardToInPlayCards(card->card);
            _MoveNewCardToSet(card, { cards::CardSets::IN_PLAY, -1 });
            break;
        }
        case UIEvent::HAND_REVEAL_STATE_CHANGED:
        {
            HandRevealStateChangedEvent handRevealStateChangedEvent = std::any_cast<HandRevealStateChangedEvent>(event.event);
            _uiState.players[handRevealStateChangedEvent.playerIndex].handRevealed = handRevealStateChangedEvent.isRevealed;
            break;
        }
        case UIEvent::DECK_SHUFFLED:
        {
            DeckShuffledEvent deckShuffledEvent = std::any_cast<DeckShuffledEvent>(event.event);
            auto& deckRef = _uiState.GetDeck(deckShuffledEvent.deck);
            std::shuffle(deckRef.begin(), deckRef.end(), deckShuffledEvent.engine);
            break;
        }
        default:
            continue;
        }

        _prevEventTime = ztime::Main();
        _minTimeUntilNextEvent = GetEventTimings(event.type).postEvent;
    }
}

void zcom::Board::_ProcessCardMove(_Card* card, UIEvent pairedEvent)
{
    if (pairedEvent.type == UIEvent::CARD_ENTER_HAND)
    {
        auto event = std::any_cast<CardEnterHandEvent>(pairedEvent.event);
        _uiState.AddCardToHand(card->card, event.playerIndex);
        _MoveCardToSet(card, { cards::CardSets::HAND, event.playerIndex });
    }
    else if (pairedEvent.type == UIEvent::CARD_ENTER_ACTIVES)
    {
        auto event = std::any_cast<CardEnterActivesEvent>(pairedEvent.event);
        _uiState.AddCardToActiveCards(card->card, event.playerIndex);
        _MoveCardToSet(card, { cards::CardSets::ACTIVE_CARDS, event.playerIndex });
    }
    else if (pairedEvent.type == UIEvent::CARD_ENTER_DECK)
    {
        _uiState.AddCardToDeck(card->card);
        _MoveCardToSet(card, { cards::CardSets::DECK, -1 });
    }
    else if (pairedEvent.type == UIEvent::CARD_ENTER_GRAVEYARD)
    {
        _uiState.AddCardToGraveyard(card->card);
        _MoveCardToSet(card, { cards::CardSets::GRAVEYARD, -1 });
    }
    else if (pairedEvent.type == UIEvent::CARD_ENTER_DESTROYED_CARDS)
    {
        _uiState.AddCardToDestroyedCards(card->card);
        _MoveCardToSet(card, { cards::CardSets::DESTROYED, -1 });
    }
    else if (pairedEvent.type == UIEvent::CARD_ENTER_IN_PLAY_CARDS)
    {
        _uiState.AddCardToInPlayCards(card->card);
        _MoveCardToSet(card, { cards::CardSets::IN_PLAY, -1 });
    }
}

void zcom::Board::_MoveCardToSet(_Card* card, cards::CardSet newSet)
{
    card->startXPos = card->xPos;
    card->startYPos = card->yPos;
    card->startRotation = card->rotation;
    card->startOpacity = card->opacity;
    card->moving = true;
    card->moveStartTime = ztime::Main();
    card->moveDuration = Duration(600, MILLISECONDS);
    card->set = newSet;
    _CalculateCardTargetPositions();
}

void zcom::Board::_MoveNewCardToSet(_Card* card, cards::CardSet set)
{
    card->set = set;
    card->startOpacity = 0.0f;
    card->moving = true;
    card->moveStartTime = ztime::Main();
    card->moveDuration = Duration(600, MILLISECONDS);
    _CalculateCardTargetPositions();
    // Set start layout after target layout is calculated
    card->startXPos = card->targetXPos;
    card->startYPos = card->targetYPos;
    card->startRotation = card->targetRotation;
}

void zcom::Board::EnableNoClickMode()
{
    _noClickMode = true;
    if (_heldCard)
    {
        _Card* card = _GetCardFromPointer(_heldCard);
        card->startXPos = GetMousePosX();
        card->startYPos = GetMousePosY();
        card->startRotation = 0.0f;
        card->targetXPos = card->xPos;
        card->targetYPos = card->yPos;
        card->targetRotation = card->rotation;
        card->moving = true;
        card->moveStartTime = ztime::Main();
        card->moveDuration = Duration(300, MILLISECONDS);
        _heldCard = nullptr;
    }
}

void zcom::Board::DisableNoClickMode()
{
    _noClickMode = false;
}

void zcom::Board::EnableChooseCardFromHandMode(int choosingPlayerIndex, int targetPlayerIndex, int maxCards, std::vector<cards::CardType> allowedTypes, std::function<void(cards::Card*)> onCardSelect, std::function<void(cards::Card*)> onCardDeselect)
{
    _chooseCardsFromHandMode = true;
    _chooseCardsFromHandChoosingPlayerIndex = choosingPlayerIndex;
    _chooseCardsFromHandTargetPlayerIndex = targetPlayerIndex;
    _chooseCardsFromHandMaxCardCount = maxCards;
    _chooseCardsFromHandAllowedTypes = allowedTypes;
    _selectedCardInHandHandler = onCardSelect;
    _deselectedCardInHandHandler = onCardDeselect;

    if (_heldCard)
    {
        _Card* card = _GetCardFromPointer(_heldCard);
        card->startXPos = GetMousePosX();
        card->startYPos = GetMousePosY();
        card->startRotation = 0.0f;
        card->targetXPos = card->xPos;
        card->targetYPos = card->yPos;
        card->targetRotation = card->rotation;
        card->moving = true;
        card->moveStartTime = ztime::Main();
        card->moveDuration = Duration(300, MILLISECONDS);
        _heldCard = nullptr;
        _DetectHoveredCard();
    }
}

void zcom::Board::DisableChooseCardFromHandMode()
{
    _chooseCardsFromHandMode = false;
    _DetectHoveredCard();
}

void zcom::Board::EnableChooseCardFromActiveCardsMode(int choosingPlayerIndex, int targetPlayerIndex, int maxCards, std::vector<cards::CardType> allowedTypes, std::function<void(cards::Card*)> onCardSelect, std::function<void(cards::Card*)> onCardDeselect)
{
    _chooseCardsFromActiveCardsMode = true;
    _chooseCardsFromActiveCardsChoosingPlayerIndex = choosingPlayerIndex;
    _chooseCardsFromActiveCardsTargetPlayerIndex = targetPlayerIndex;
    _chooseCardsFromActiveCardsMaxCardCount = maxCards;
    _chooseCardsFromActiveCardsAllowedTypes = allowedTypes;
    _selectedCardInActiveCardsHandler = onCardSelect;
    _deselectedCardInActiveCardsHandler = onCardDeselect;

    if (_heldCard)
    {
        _Card* card = _GetCardFromPointer(_heldCard);
        card->startXPos = GetMousePosX();
        card->startYPos = GetMousePosY();
        card->startRotation = 0.0f;
        card->targetXPos = card->xPos;
        card->targetYPos = card->yPos;
        card->targetRotation = card->rotation;
        card->moving = true;
        card->moveStartTime = ztime::Main();
        card->moveDuration = Duration(300, MILLISECONDS);
        _heldCard = nullptr;
        _DetectHoveredCard();
    }
}

void zcom::Board::DisableChooseCardFromActiveCardsMode()
{
    _chooseCardsFromActiveCardsMode = false;
    _DetectHoveredCard();
}

void zcom::Board::EnableChooseCardFromDisplayedCardsMode(int playerIndex, int maxCards, std::vector<cards::CardType> allowedTypes, std::function<void(cards::Card*)> onCardSelect, std::function<void(cards::Card*)> onCardDeselect)
{
    _chooseCardsFromDisplayedCardsMode = true;
    _chooseCardsFromDisplayedCardsPlayerIndex = playerIndex;
    _chooseCardsFromDisplayedCardsMaxCardCount = maxCards;
    _chooseCardsFromDisplayedCardsAllowedTypes = allowedTypes;
    _selectedCardInDisplayedCardsHandler = onCardSelect;
    _deselectedCardInDisplayedCardsHandler = onCardDeselect;

    if (_heldCard)
    {
        _Card* card = _GetCardFromPointer(_heldCard);
        card->startXPos = GetMousePosX();
        card->startYPos = GetMousePosY();
        card->startRotation = 0.0f;
        card->targetXPos = card->xPos;
        card->targetYPos = card->yPos;
        card->targetRotation = card->rotation;
        card->moving = true;
        card->moveStartTime = ztime::Main();
        card->moveDuration = Duration(300, MILLISECONDS);
        _heldCard = nullptr;
        _DetectHoveredCard();
    }
}

void zcom::Board::DisableChooseCardFromDisplayedCardsMode()
{
    _chooseCardsFromActiveCardsMode = false;
    _DetectHoveredCard();
}

void zcom::Board::EnableDrawCardMode(std::vector<cards::CardType> allowedTypes, std::function<void(cards::Card*)> onCardDraw)
{
    _drawCardMode = true;
    _drawCardHandler = onCardDraw;

    if (_heldCard)
    {
        _Card* card = _GetCardFromPointer(_heldCard);
        if (card->set.set == cards::CardSets::DECK)
            return;

        card->startXPos = GetMousePosX();
        card->startYPos = GetMousePosY();
        card->startRotation = 0.0f;
        card->targetXPos = card->xPos;
        card->targetYPos = card->yPos;
        card->targetRotation = card->rotation;
        card->moving = true;
        card->moveStartTime = ztime::Main();
        card->moveDuration = Duration(300, MILLISECONDS);
        _heldCard = nullptr;
    }
    _DetectHoveredCard();
}

void zcom::Board::DisableDrawCardMode()
{
    _drawCardMode = false;
    _drawCardHandler = {};

    if (!_heldCard)
        _DetectHoveredCard();
}