#include "Board.h"
#include "App.h"

void zcom::Board::_OnUpdate()
{

}

void zcom::Board::_OnDraw(Graphics g)
{
    float padding = std::ceilf(shadowRadius);

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
            card.targetXPos - CARD_WIDTH / 2 - padding,
            card.targetYPos - CARD_HEIGHT / 2 - padding,
            card.targetXPos + CARD_WIDTH / 2 + padding,
            card.targetYPos + CARD_HEIGHT / 2 + padding
        );

        g.target->DrawBitmap(_offenseCardBitmap);
        HRESULT hr = g.target->Flush();
        if (hr != S_OK)
        {
            std::cout << "?\n";
        }

        //g.target->SetTransform(D2D1::Matrix3x2F::Rotation(card.targetRotation * RADIAN, { card.targetXPos, card.targetYPos }));
        //if (card.card->GetCardType() == cards::CardType::OFFENSE)
        //    g.target->DrawBitmap(_offenseCardBitmap, &rect);
        //else if (card.card->GetCardType() == cards::CardType::DEFENSE)
        //    g.target->DrawBitmap(_defenseCardBitmap, &rect);
        //else if (card.card->GetCardType() == cards::CardType::UTILITY)
        //    g.target->DrawBitmap(_utilityCardBitmap, &rect);
        //else if (card.card->GetCardType() == cards::CardType::COMBO)
        //    g.target->DrawBitmap(_comboCardBitmap, &rect);
        //g.target->SetTransform(D2D1::Matrix3x2F::Identity());

        //g.target->FillRoundedRectangle(roundedRect, fillBrush);
        //g.target->DrawRoundedRectangle(roundedRect, borderBrush, 2.0f);
    }
}

void zcom::Board::_GenerateCardBitmap(Graphics g, ID2D1Bitmap** bitmapRef, D2D1_COLOR_F color)
{
    float padding = std::ceilf(shadowRadius);

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

    D2D1_RECT_F rect = D2D1::RectF(
        padding,
        padding,
        CARD_WIDTH + padding,
        CARD_HEIGHT + padding
    );
    D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(rect, 5.0f, 5.0f);

    g.target->GetTarget(&stash);
    g.target->SetTarget(contentBitmap);
    g.target->Clear();
    g.target->FillRoundedRectangle(roundedRect, fillBrush);
    g.target->SetTarget(stash);
    stash->Release();

    // Compose bitmap with shadow
    //ID2D1Bitmap1* compositeBitmap = nullptr;
    //g.target->CreateBitmap(
    //    D2D1::SizeU(CARD_WIDTH + padding * 2, CARD_HEIGHT + padding * 2),
    //    nullptr,
    //    0,
    //    D2D1::BitmapProperties1(
    //        D2D1_BITMAP_OPTIONS_TARGET,
    //        { DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED }
    //    ),
    //    &compositeBitmap
    //);

    //ID2D1Effect* shadowEffect = nullptr;
    //g.target->CreateEffect(CLSID_D2D1Shadow, &shadowEffect);
    //shadowEffect->SetInput(0, contentBitmap);
    //shadowEffect->SetValue(D2D1_SHADOW_PROP_COLOR, D2D1::Vector4F(0.0f, 0.0f, 0.0f, 0.8f));
    //shadowEffect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, shadowRadius);

    //g.target->GetTarget(&stash);
    //g.target->SetTarget(compositeBitmap);
    //g.target->Clear();
    //g.target->DrawImage(shadowEffect, D2D1::Point2F(0.0f, 0.0f));
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
        HRESULT hr = (*bitmapRef)->CopyFromBitmap(&point, contentBitmap, &rect);
        hr = g.target->Flush();
        //HRESULT hr = (*bitmapRef)->CopyFromBitmap(&point, compositeBitmap, &rect);
        if (hr != S_OK)
        {
            std::cout << "!\n";
        }
    }

    fillBrush->Release();
    //shadowEffect->Release();
    //compositeBitmap->Release();
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
                    card.zIndex = core->GetState().offenseDeck.size() - i - 1;
                    card.set.set = cards::CardSets::DECK;
                    card.targetXPos = offenseDeckCenterPosX - card.zIndex;
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
                    card.zIndex = core->GetState().defenseDeck.size() - i - 1;
                    card.set.set = cards::CardSets::DECK;
                    card.targetXPos = defenseDeckCenterPosX - card.zIndex;
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
                    card.zIndex = core->GetState().utilityDeck.size() - i - 1;
                    card.set.set = cards::CardSets::DECK;
                    card.targetXPos = utilityDeckCenterPosX - card.zIndex;
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
                    card.zIndex = core->GetState().comboDeck.size() - i - 1;
                    card.set.set = cards::CardSets::DECK;
                    card.targetXPos = comboDeckCenterPosX - card.zIndex;
                    card.targetYPos = comboDeckCenterPosY;
                    card.targetRotation = PI / 2;
                    break;
                }
            }
        }
    }

    for (auto& card : _cards)
    {
        cards::CardSet set = core->GetCardSet(card.card);
        switch (set.set)
        {
        case cards::CardSets::DECK:
        {

        }
        default:
        {
            // 
            break;
        }
        }
    }
}