#include "Board.h"
#include "App.h"

#include "Helper/Functions.h"

void zcom::Board::_OnUpdate()
{

}

void zcom::Board::_OnDraw(Graphics g)
{
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
    // Move hovered card to end
    auto it = std::find_if(_cards.begin(), _cards.end(), [&](const _Card& card) { return card.card == _hoveredCard; });
    if (it != _cards.end())
    {
        _Card card = *it;
        _cards.erase(it);
        _cards.insert(_cards.end(), card);
    }
    //g.target->DrawBitmap(_offenseCardBitmap);

    for (auto& card : _cards)
    {
        //ID2D1SolidColorBrush* fillBrush = nullptr;
        //ID2D1SolidColorBrush* borderBrush = nullptr;
        //if (card.card->GetCardType() == cards::CardType::OFFENSE)
        //{
        //    g.target->CreateSolidColorBrush(D2D1::ColorF(0.79f, 0.22f, 0.14f), &fillBrush);
        //    g.target->CreateSolidColorBrush(D2D1::ColorF(0.10f, 0.02f, 0.01f), &borderBrush);
        //}
        //else if (card.card->GetCardType() == cards::CardType::DEFENSE)
        //{
        //    g.target->CreateSolidColorBrush(D2D1::ColorF(0.27f, 0.70f, 0.29f), &fillBrush);
        //    g.target->CreateSolidColorBrush(D2D1::ColorF(0.04f, 0.08f, 0.04f), &borderBrush);
        //}
        //else if (card.card->GetCardType() == cards::CardType::UTILITY)
        //{
        //    g.target->CreateSolidColorBrush(D2D1::ColorF(0.24f, 0.50f, 0.75f), &fillBrush);
        //    g.target->CreateSolidColorBrush(D2D1::ColorF(0.03f, 0.06f, 0.09f), &borderBrush);
        //}
        //else if (card.card->GetCardType() == cards::CardType::COMBO)
        //{
        //    g.target->CreateSolidColorBrush(D2D1::ColorF(0.18f, 0.17f, 0.15f), &fillBrush);
        //    g.target->CreateSolidColorBrush(D2D1::ColorF(0.02f, 0.02f, 0.02f), &borderBrush);
        //}

        //D2D1_RECT_F rect = D2D1::RectF(
        //    card.targetXPos - CARD_WIDTH / 2,
        //    card.targetYPos - CARD_HEIGHT / 2,
        //    card.targetXPos + CARD_WIDTH / 2,
        //    card.targetYPos + CARD_HEIGHT / 2
        //);
        //D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(rect, 5.0f, 5.0f);

        //g.target->SetTransform(D2D1::Matrix3x2F::Rotation(card.targetRotation * RADIAN, { card.targetXPos, card.targetYPos }));
        //g.target->FillRoundedRectangle(roundedRect, fillBrush);
        //g.target->DrawRoundedRectangle(roundedRect, borderBrush, 2.0f);
        //g.target->SetTransform(D2D1::Matrix3x2F::Identity());

        //fillBrush->Release();
        //borderBrush->Release();

        D2D1_RECT_F rect = D2D1::RectF(
            card.targetXPos - (CARD_WIDTH / 2 - padding) * card.scale,
            card.targetYPos - (CARD_HEIGHT / 2 - padding) * card.scale,
            card.targetXPos + (CARD_WIDTH / 2 + padding) * card.scale,
            card.targetYPos + (CARD_HEIGHT / 2 + padding) * card.scale
        );

        //g.target->DrawBitmap(_offenseCardBitmap, rect);
        //HRESULT hr = g.target->Flush();
        //if (hr != S_OK)
        //{
        //    std::cout << "?\n";
        //}

        g.target->SetTransform(D2D1::Matrix3x2F::Rotation(-card.targetRotation * RADIAN, { card.targetXPos, card.targetYPos }));
        if (card.card->GetCardType() == cards::CardType::OFFENSE)
            g.target->DrawBitmap(_offenseCardBitmap, &rect);
        else if (card.card->GetCardType() == cards::CardType::DEFENSE)
            g.target->DrawBitmap(_defenseCardBitmap, &rect);
        else if (card.card->GetCardType() == cards::CardType::UTILITY)
            g.target->DrawBitmap(_utilityCardBitmap, &rect);
        else if (card.card->GetCardType() == cards::CardType::COMBO)
            g.target->DrawBitmap(_comboCardBitmap, &rect);
        g.target->SetTransform(D2D1::Matrix3x2F::Identity());

        //g.target->FillRoundedRectangle(roundedRect, fillBrush);
        //g.target->DrawRoundedRectangle(roundedRect, borderBrush, 2.0f);
    }
}

zcom::EventTargets zcom::Board::_OnMouseMove(int deltaX, int deltaY)
{
    _hoveredCard = _GetHoveredCard();
    return EventTargets().Add(this, GetMousePosX(), GetMousePosY());
}

void zcom::Board::_OnMouseEnterArea()
{

}

void zcom::Board::_OnMouseLeaveArea()
{
    _hoveredCard = nullptr;
}

void zcom::Board::_GenerateCardBitmap(Graphics g, ID2D1Bitmap** bitmapRef, D2D1_COLOR_F color)
{
    //float padding = 100.0f;
    //float padding = std::ceilf(shadowRadius);
    float padding = 0.0f;

    ID2D1Image* stash = nullptr;

    // Create bitmap of card
    ID2D1Bitmap1* contentBitmap = nullptr;
    g.target->CreateBitmap(
        D2D1::SizeU(CARD_WIDTH + padding * 2, CARD_HEIGHT + padding * 2),
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
        padding + 1.0f,
        padding + 1.0f,
        CARD_WIDTH + padding - 1.0f,
        CARD_HEIGHT + padding - 1.0f
    );
    D2D1_ROUNDED_RECT roundedCardRect = D2D1::RoundedRect(cardRect, 5.0f, 5.0f);

    D2D1_RECT_F borderRect = D2D1::RectF(
        padding + 0.5f,
        padding + 0.5f,
        CARD_WIDTH + padding - 0.5f,
        CARD_HEIGHT + padding - 0.5f
    );
    D2D1_ROUNDED_RECT roundedBorderRect = D2D1::RoundedRect(borderRect, 5.0f, 5.0f);

    g.target->GetTarget(&stash);
    g.target->SetTarget(contentBitmap);
    g.target->Clear();
    g.target->FillRoundedRectangle(roundedCardRect, fillBrush);
    g.target->DrawRoundedRectangle(roundedBorderRect, borderBrush);
    g.target->SetTarget(stash);
    stash->Release();

    //// Create shadow bitmap
    //ID2D1Bitmap1* shadowBitmap = nullptr;
    //g.target->CreateBitmap(
    //    D2D1::SizeU(CARD_WIDTH + padding * 2, CARD_HEIGHT + padding * 2),
    //    nullptr,
    //    0,
    //    D2D1::BitmapProperties1(
    //        D2D1_BITMAP_OPTIONS_TARGET,
    //        { DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED }
    //    ),
    //    &shadowBitmap
    //);

    //ID2D1Effect* shadowEffect = nullptr;
    //g.target->CreateEffect(CLSID_D2D1Shadow, &shadowEffect);
    //shadowEffect->SetInput(0, contentBitmap);
    //shadowEffect->SetValue(D2D1_SHADOW_PROP_COLOR, D2D1::Vector4F(0.1f, 0.1f, 0.1f, 1.0f));
    //shadowEffect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, shadowRadius);

    //g.target->GetTarget(&stash);
    //g.target->SetTarget(shadowBitmap);
    //g.target->Clear();
    //g.target->DrawImage(shadowEffect, D2D1::Point2F(0.0f, 0.0f));
    //g.target->DrawBitmap(contentBitmap);
    //g.target->SetTarget(stash);
    //stash->Release();

    // Create final bitmap
    g.target->CreateBitmap(
        D2D1::SizeU(CARD_WIDTH + padding * 2, CARD_HEIGHT + padding * 2),
        nullptr,
        0,
        D2D1::BitmapProperties({ DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED }),
        bitmapRef
    );

    {
        D2D1_POINT_2U point = D2D1::Point2U();
        D2D1_RECT_U rect = D2D1::RectU(0, 0, CARD_WIDTH + padding * 2, CARD_HEIGHT + padding * 2);
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

void zcom::Board::_CreateMissingCards()
{
    for (auto& cardPtr : core->GetState().offenseDeck)
    {
        auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
        if (it == _cards.end())
            _cards.push_back(_Card{ cardPtr.get() });
    }
    for (auto& cardPtr : core->GetState().defenseDeck)
    {
        auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
        if (it == _cards.end())
            _cards.push_back(_Card{ cardPtr.get() });
    }
    for (auto& cardPtr : core->GetState().utilityDeck)
    {
        auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
        if (it == _cards.end())
            _cards.push_back(_Card{ cardPtr.get() });
    }
    for (auto& cardPtr : core->GetState().comboDeck)
    {
        auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
        if (it == _cards.end())
            _cards.push_back(_Card{ cardPtr.get() });
    }
    for (auto& player : core->GetState().players)
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
    for (auto& cardPtr : core->GetState().graveyard)
    {
        auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
        if (it == _cards.end())
            _cards.push_back(_Card{ cardPtr.get() });
    }
    for (auto& cardPtr : core->GetState().inPlayCards)
    {
        auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
        if (it == _cards.end())
            _cards.push_back(_Card{ cardPtr.get() });
    }
    for (auto& cardPtr : core->GetState().destroyedCards)
    {
        auto it = std::find_if(_cards.begin(), _cards.end(), [&](_Card& card) { return card.card == cardPtr.get(); });
        if (it == _cards.end())
            _cards.push_back(_Card{ cardPtr.get() });
    }
}

void zcom::Board::_CalculateCardTargetPositions()
{
    float viewWidth = GetWidth();
    float viewHeight = GetHeight();

    float gapFromScreenEdge = 100.0f;
    float gapBetweenDecks = 30.0f;

    for (auto& card : _cards)
    {
        card.targetXPos = viewWidth / 2;
        card.targetYPos = viewHeight / 2;
        if (card.card == _hoveredCard)
            card.scale = 1.25f;
        else
            card.scale = 1.0f;
    }

    // Calculate deck positions
    {
        float offenseDeckCenterPosX = gapFromScreenEdge + CARD_HEIGHT / 2;
        float offenseDeckCenterPosY = viewHeight / 2 - gapBetweenDecks * 1.5f - CARD_WIDTH * 1.5f;
        float defenseDeckCenterPosX = gapFromScreenEdge + CARD_HEIGHT / 2;
        float defenseDeckCenterPosY = viewHeight / 2 - gapBetweenDecks * 0.5f - CARD_WIDTH * 0.5f;
        float utilityDeckCenterPosX = gapFromScreenEdge + CARD_HEIGHT / 2;
        float utilityDeckCenterPosY = viewHeight / 2 + gapBetweenDecks * 0.5f + CARD_WIDTH * 0.5f;
        float comboDeckCenterPosX = gapFromScreenEdge + CARD_HEIGHT / 2;
        float comboDeckCenterPosY = viewHeight / 2 + gapBetweenDecks * 1.5f + CARD_WIDTH * 1.5f;

        for (int i = 0; i < core->GetState().offenseDeck.size(); i++)
        {
            for (auto& card : _cards)
            {
                if (card.card == core->GetState().offenseDeck[i].get())
                {
                    card.zIndex = i;
                    card.set.set = cards::CardSets::DECK;
                    card.targetXPos = offenseDeckCenterPosX - i * 1.25f;
                    card.targetYPos = offenseDeckCenterPosY;
                    card.targetRotation = PI / 2;
                    break;
                }
            }
        }
        for (int i = 0; i < core->GetState().defenseDeck.size(); i++)
        {
            for (auto& card : _cards)
            {
                if (card.card == core->GetState().defenseDeck[i].get())
                {
                    card.zIndex = i;
                    card.set.set = cards::CardSets::DECK;
                    card.targetXPos = defenseDeckCenterPosX - i * 1.25f;
                    card.targetYPos = defenseDeckCenterPosY;
                    card.targetRotation = PI / 2;
                    break;
                }
            }
        }
        for (int i = 0; i < core->GetState().utilityDeck.size(); i++)
        {
            for (auto& card : _cards)
            {
                if (card.card == core->GetState().utilityDeck[i].get())
                {
                    card.zIndex = i;
                    card.set.set = cards::CardSets::DECK;
                    card.targetXPos = utilityDeckCenterPosX - i * 1.25f;
                    card.targetYPos = utilityDeckCenterPosY;
                    card.targetRotation = PI / 2;
                    break;
                }
            }
        }
        for (int i = 0; i < core->GetState().comboDeck.size(); i++)
        {
            for (auto& card : _cards)
            {
                if (card.card == core->GetState().comboDeck[i].get())
                {
                    card.zIndex = i;
                    card.set.set = cards::CardSets::DECK;
                    card.targetXPos = comboDeckCenterPosX - i * 1.25f;
                    card.targetYPos = comboDeckCenterPosY;
                    card.targetRotation = PI / 2;
                    break;
                }
            }
        }
    }

    // Calculate hand card positions
    {
        { // Player 1
            int cardsInHand = core->GetState().players[0].hand.size();
            Pos2D<float> circleCenter = { viewWidth / 2, viewHeight + 550.0f };
            Pos2D<float> circleTop = circleCenter + Pos2D<float>(0.0f, -700.0f);
            float gapBetweenCards = 0.2f;

            for (int i = 0; i < cardsInHand; i++)
            {
                for (auto& card : _cards)
                {
                    if (card.card == core->GetState().players[0].hand[i].get())
                    {
                        float cardAngle = (gapBetweenCards * (cardsInHand - 1)) / 2 - gapBetweenCards * i;
                        Pos2D<float> cardPos = point_rotated_by(circleCenter, circleTop, cardAngle);

                        card.zIndex = i;
                        card.set.set = cards::CardSets::HAND;
                        card.targetXPos = cardPos.x;
                        card.targetYPos = cardPos.y;
                        card.targetRotation = cardAngle;
                        //card.targetRotation = 0.1f;
                        break;
                    }
                }
            }
        }
        { // Player 2
            int cardsInHand = core->GetState().players[1].hand.size();
            Pos2D<float> circleCenter = { viewWidth / 2, -550.0f };
            Pos2D<float> circleBottom = circleCenter + Pos2D<float>(0.0f, 700.0f);
            float startAngle = PI;
            float gapBetweenCards = 0.2f;

            for (int i = 0; i < cardsInHand; i++)
            {
                for (auto& card : _cards)
                {
                    if (card.card == core->GetState().players[1].hand[i].get())
                    {
                        float cardAngle = (gapBetweenCards * (cardsInHand - 1)) / 2 - gapBetweenCards * i;
                        Pos2D<float> cardPos = point_rotated_by(circleCenter, circleBottom, cardAngle);

                        card.zIndex = i;
                        card.set.set = cards::CardSets::HAND;
                        card.targetXPos = cardPos.x;
                        card.targetYPos = cardPos.y;
                        card.targetRotation = startAngle + cardAngle;
                        //card.targetRotation = 0.1f;
                        break;
                    }
                }
            }
        }
    }


    // Calculate graveyard card positions
    {
        float graveyardDeckCenterPosX = viewWidth - gapFromScreenEdge - CARD_WIDTH / 2;
        float graveyardDeckCenterPosY = viewHeight / 2;

        for (int i = 0; i < core->GetState().graveyard.size(); i++)
        {
            for (auto& card : _cards)
            {
                if (card.card == core->GetState().graveyard[i].get())
                {
                    card.zIndex = i;
                    card.set.set = cards::CardSets::GRAVEYARD;
                    card.targetXPos = graveyardDeckCenterPosX + i * 1.25f;
                    card.targetYPos = graveyardDeckCenterPosY;
                    card.targetRotation = 0.0f;
                    break;
                }
            }
        }
    }
}

cards::Card* zcom::Board::_GetHoveredCard()
{
    std::vector<_Card> sortedCards = _cards;
    std::sort(sortedCards.begin(), sortedCards.end(), [&](const _Card& card1, const _Card& card2) { return card1.zIndex > card2.zIndex; });
    // Move hovered card to beggining
    auto it = std::find_if(sortedCards.begin(), sortedCards.end(), [&](const _Card& card) { return card.card == _hoveredCard; });
    if (it != sortedCards.end())
    {
        _Card card = *it;
        sortedCards.erase(it);
        sortedCards.insert(sortedCards.begin(), card);
    }

    for (auto& card : sortedCards)
    {
        // Skip cards not on top of decks
        bool skip = false;
        for (int i = 0; i < core->GetState().offenseDeck.size() - 1; i++)
        {
            if (card.card == core->GetState().offenseDeck[i].get())
            {
                skip = true;
                break;
            }
        }
        if (skip)
            continue;
        for (int i = 0; i < core->GetState().defenseDeck.size() - 1; i++)
        {
            if (card.card == core->GetState().defenseDeck[i].get())
            {
                skip = true;
                break;
            }
        }
        if (skip)
            continue;
        for (int i = 0; i < core->GetState().utilityDeck.size() - 1; i++)
        {
            if (card.card == core->GetState().utilityDeck[i].get())
            {
                skip = true;
                break;
            }
        }
        if (skip)
            continue;
        for (int i = 0; i < core->GetState().comboDeck.size() - 1; i++)
        {
            if (card.card == core->GetState().comboDeck[i].get())
            {
                skip = true;
                break;
            }
        }
        if (skip)
            continue;
        for (int i = 0; i < core->GetState().graveyard.size() - 1; i++)
        {
            if (card.card == core->GetState().graveyard[i].get())
            {
                skip = true;
                break;
            }
        }
        if (skip)
            continue;

        Pos2D<float> mousePos = { (float)GetMousePosX(), (float)GetMousePosY() };
        Pos2D<float> cardCenterPos = { card.targetXPos, card.targetYPos };
        // Rotate mouse position around card center to align relative coordinates to upright card position
        Pos2D<float> adjustedMousePos = point_rotated_by(cardCenterPos, mousePos, -card.targetRotation);

        float cardLeftSide = card.targetXPos - (CARD_WIDTH / 2) * card.scale;
        float cardRightSide = card.targetXPos + (CARD_WIDTH / 2) * card.scale;
        float cardTopSide = card.targetYPos - (CARD_HEIGHT / 2) * card.scale;
        float cardBottomSide = card.targetYPos + (CARD_HEIGHT / 2) * card.scale;
        if (adjustedMousePos.x >= cardLeftSide && adjustedMousePos.x <= cardRightSide &&
            adjustedMousePos.y >= cardTopSide && adjustedMousePos.y <= cardBottomSide)
        {
            return card.card;
        }
    }

    return nullptr;
}