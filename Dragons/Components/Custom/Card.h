#pragma once

#include "Components/Base/ComponentBase.h"
#include "Components/Base/Panel.h"
#include "Components/Base/Label.h"
#include "Components/Base/Image.h"
#include "Components/Base/EmptyPanel.h"

#include "Core/Cards/Card.h"
#include "Core/Cards/CardPosition.h"

namespace zcom
{
    class Card : public Base
    {
    public:
        ID2D1Bitmap* RenderCardBitmap(Graphics g)
        {
            _cardPanel->Resize(CARD_WIDTH, CARD_HEIGHT);
            _cardPanel->Draw(g);

            // Create final bitmap
            ID2D1Bitmap* cardBitmap = nullptr;
            g.target->CreateBitmap(
                D2D1::SizeU(CARD_WIDTH, CARD_HEIGHT),
                nullptr,
                0,
                D2D1::BitmapProperties({ DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED }),
                &cardBitmap
            );

            D2D1_POINT_2U point = D2D1::Point2U();
            D2D1_RECT_U rect = D2D1::RectU(0, 0, CARD_WIDTH, CARD_HEIGHT);
            ID2D1Image* stash = nullptr;
            g.target->GetTarget(&stash);
            g.target->SetTarget(_cardPanel->Image());
            cardBitmap->CopyFromRenderTarget(&point, g.target, &rect);
            g.target->SetTarget(stash);
            stash->Release();

            return cardBitmap;
        }

#pragma region base_class
    protected:
    public:
        const char* GetName() const { return Name(); }
        static const char* Name() { return "card"; }
#pragma endregion

    private:
        cards::Card* _card;

        std::unique_ptr<Panel> _cardPanel;
        std::unique_ptr<Label> _title;
        std::unique_ptr<zcom::Image> _image;
        std::unique_ptr<Label> _flavorText;
        std::unique_ptr<Label> _description;

        std::unique_ptr<EmptyPanel> _border;
        std::unique_ptr<EmptyPanel> _background;

        float CARD_WIDTH = 600.0f;
        float CARD_HEIGHT = 800.0f;
        float BORDER_WIDTH = 4.0f;

    protected:
        friend class Scene;
        friend class Base;
        Card(Scene* scene) : Base(scene) {}
        void Init(cards::Card* card)
        {
            D2D1_COLOR_F textColor = D2D1::ColorF(0.1f, 0.1f, 0.1f);

            _card = card;

            _title = Create<Label>(_card->GetCardName());
            _title->SetVerticalOffsetPixels(10.0f);
            _title->SetParentWidthPercent(1.0f);
            _title->SetBaseWidth(-BORDER_WIDTH * 2);
            _title->SetHorizontalOffsetPixels(BORDER_WIDTH);
            _title->SetBaseHeight(64.0f);
            _title->SetFont(L"Times New Roman");
            _title->SetFontSize(48.0f);
            _title->SetFontColor(textColor);
            _title->SetMargins({ 20.0f, 0.0f, 0.0f, 0.0f });
            _title->SetVerticalTextAlignment(zcom::Alignment::CENTER);
            _title->SetBackgroundColor(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.4f));

            _image = Create<zcom::Image>(nullptr);
            _image->SetOffsetPixels(5.0f, 79.0f);
            _image->SetBaseSize(590, 400);
            _image->SetBorderVisibility(true);
            _image->SetBorderWidth(5.0f);
            _image->SetBorderColor(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.8f));

            //_flavorText = Create<Label>(L"Flavor text goes here and is very tasty");
            _flavorText = Create<Label>(L"A stab from this sword, will leave a wound in your soul");
            _flavorText->SetVerticalOffsetPixels(484.0f);
            _flavorText->SetParentWidthPercent(1.0f);
            _flavorText->SetBaseWidth(-BORDER_WIDTH * 2);
            _flavorText->SetHorizontalOffsetPixels(BORDER_WIDTH);
            _flavorText->SetBaseHeight(48.0f);
            _flavorText->SetFont(L"Times New Roman");
            _flavorText->SetFontSize(25.0f);
            _flavorText->SetFontColor(textColor);
            _flavorText->SetMargins({ 16.0f, 0.0f, 0.0f, 0.0f });
            _flavorText->SetVerticalTextAlignment(zcom::Alignment::CENTER);
            _flavorText->SetBackgroundColor(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.4f));

            _description = Create<Label>(L"Descriptions goes here\nMultiple lines aswell lmao");
            _description->SetOffsetPixels(5.0f, 537.0f);
            _description->SetBaseSize(590, 240);
            _description->SetBorderVisibility(true);
            _description->SetBorderWidth(5.0f);
            _description->SetBorderColor(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.8f));
            _description->SetFont(L"Times New Roman");
            _description->SetFontSize(26.0f);
            _description->SetFontColor(textColor);
            _description->SetFontStyle(DWRITE_FONT_STYLE_ITALIC);
            _description->SetMargins({ 15.0f, 10.0f, 10.0f, 10.0f });
            _description->SetBackgroundColor(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.4f));

            D2D1_COLOR_F borderColor;
            D2D1_COLOR_F backgroundColor;
            if (_card->GetCardType() == cards::CardType::OFFENSE)
                backgroundColor = D2D1::ColorF(0.79f, 0.22f, 0.14f);
            if (_card->GetCardType() == cards::CardType::DEFENSE)
                backgroundColor = D2D1::ColorF(0.27f, 0.70f, 0.29f);
            if (_card->GetCardType() == cards::CardType::UTILITY)
                backgroundColor = D2D1::ColorF(0.24f, 0.50f, 0.75f);
            if (_card->GetCardType() == cards::CardType::COMBO)
                backgroundColor = D2D1::ColorF(0.18f, 0.17f, 0.15f);

            borderColor = backgroundColor;
            borderColor.r *= 0.5f;
            borderColor.g *= 0.5f;
            borderColor.b *= 0.5f;
            borderColor.a = 0.8f;

            _border = Create<EmptyPanel>();
            _border->SetParentSizePercent(1.0f, 1.0f);
            _border->SetBorderColor(borderColor);
            _border->SetBorderVisibility(true);
            _border->SetBorderWidth(BORDER_WIDTH);

            _background = Create<EmptyPanel>();
            _background->SetParentSizePercent(1.0f, 1.0f);
            _background->SetBaseSize(-BORDER_WIDTH * 2, -BORDER_WIDTH * 2);
            _background->SetOffsetPixels(BORDER_WIDTH, BORDER_WIDTH);
            _background->SetBackgroundColor(backgroundColor);

            _cardPanel = Create<Panel>();
            _cardPanel->AddItem(_background.get());
            _cardPanel->AddItem(_title.get());
            _cardPanel->AddItem(_image.get());
            _cardPanel->AddItem(_flavorText.get());
            _cardPanel->AddItem(_description.get());
            _cardPanel->AddItem(_border.get());
        }
        void Init(cards::CardType type)
        {
            D2D1_COLOR_F borderColor;
            D2D1_COLOR_F backgroundColor;
            if (type == cards::CardType::OFFENSE)
                backgroundColor = D2D1::ColorF(0.79f, 0.22f, 0.14f);
            if (type == cards::CardType::DEFENSE)
                backgroundColor = D2D1::ColorF(0.27f, 0.70f, 0.29f);
            if (type == cards::CardType::UTILITY)
                backgroundColor = D2D1::ColorF(0.24f, 0.50f, 0.75f);
            if (type == cards::CardType::COMBO)
                backgroundColor = D2D1::ColorF(0.18f, 0.17f, 0.15f);

            borderColor = backgroundColor;
            borderColor.r *= 0.5f;
            borderColor.g *= 0.5f;
            borderColor.b *= 0.5f;
            borderColor.a = 0.8f;

            _border = Create<EmptyPanel>();
            _border->SetParentSizePercent(1.0f, 1.0f);
            _border->SetBorderColor(borderColor);
            _border->SetBorderVisibility(true);
            _border->SetBorderWidth(BORDER_WIDTH);

            _background = Create<EmptyPanel>();
            _background->SetParentSizePercent(1.0f, 1.0f);
            _background->SetBaseSize(-BORDER_WIDTH * 2, -BORDER_WIDTH * 2);
            _background->SetOffsetPixels(BORDER_WIDTH, BORDER_WIDTH);
            _background->SetBackgroundColor(backgroundColor);

            _cardPanel = Create<Panel>();
            _cardPanel->AddItem(_background.get());
            _cardPanel->AddItem(_border.get());
        }
    public:
        ~Card()
        {
            _cardPanel->ClearItems();
        }
        Card(Card&&) = delete;
        Card& operator=(Card&&) = delete;
        Card(const Card&) = delete;
        Card& operator=(const Card&) = delete;
    };
}