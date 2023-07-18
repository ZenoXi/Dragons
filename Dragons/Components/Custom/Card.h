#pragma once

#include "Components/Base/ComponentBase.h"
#include "Components/Base/Label.h"
#include "Components/Base/Image.h"

#include "Core/Cards/Card.h"
#include "Core/Cards/CardPosition.h"

namespace zcom
{
    class Card : public Base
    {
#pragma region base_class
    protected:
        void _OnDraw(Graphics g)
        {

        }

    public:
        const char* GetName() const { return Name(); }
        static const char* Name() { return "card"; }
#pragma endregion

    private:
        cards::Card* _card;

        std::unique_ptr<Label> _title;
        std::unique_ptr<zcom::Image> _image;
        std::unique_ptr<Label> _flavorText;
        std::unique_ptr<Label> _description;

    protected:
        friend class Scene;
        friend class Base;
        Card(Scene* scene) : Base(scene) {}
        void Init(cards::Card* card)
        {
            _card = card;

            if (_card->GetCardType() == cards::CardType::OFFENSE)
                SetBackgroundColor(D2D1::ColorF(0.79f, 0.22f, 0.14f));
            if (_card->GetCardType() == cards::CardType::DEFENSE)
                SetBackgroundColor(D2D1::ColorF(0.27f, 0.70f, 0.29f));
            if (_card->GetCardType() == cards::CardType::UTILITY)
                SetBackgroundColor(D2D1::ColorF(0.24f, 0.50f, 0.75f));
            if (_card->GetCardType() == cards::CardType::COMBO)
                SetBackgroundColor(D2D1::ColorF(0.18f, 0.17f, 0.15f));

            _title = Create<Label>(_card->GetCardName());
            _title->SetParentWidthPercent(1.0f);
            _title->SetBaseHeight(20.0f);
        }
    public:
        ~Card() {}
        Card(Card&&) = delete;
        Card& operator=(Card&&) = delete;
        Card(const Card&) = delete;
        Card& operator=(const Card&) = delete;
    };
}