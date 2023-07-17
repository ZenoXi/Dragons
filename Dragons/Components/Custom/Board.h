#pragma once

#include "Components/Base/ComponentBase.h"

#include "Helper/Event.h"

#include "Core/Core.h"
#include "Core/Cards/CardPosition.h"

namespace zcom
{
    class Board : public Base
    {
#pragma region base_class
    protected:
        bool _Redraw()
        {
            return true;
        }

        void _OnUpdate();
        void _OnDraw(Graphics g);

        EventTargets _OnMouseMove(int deltaX, int deltaY);
        void _OnMouseEnterArea();
        void _OnMouseLeaveArea();

        EventTargets _OnLeftPressed(int x, int y)
        {
            return EventTargets().Add(this, x, y);
        }

        EventTargets _OnLeftReleased(int x, int y)
        {
            return EventTargets().Add(this, x, y);
        }

    public:
        const char* GetName() const { return Name(); }
        static const char* Name() { return "board"; }
#pragma endregion

    private:
        Core* core = nullptr;

        struct _Card
        {
            cards::Card* card = nullptr;
            float xPos = 0.0f;
            float yPos = 0.0f;
            float rotation = 0.0f;
            cards::CardSet set = { cards::CardSets::NONE, -1 };
            int zIndex = 0;
            float scale = 1.0f;

            float targetXPos = 0.0f;
            float targetYPos = 0.0f;
            float targetRotation = 0.0f;
            cards::CardSet targetSet = { cards::CardSets::NONE, -1 };
            float startXPos = 0.0f;
            float startYPos = 0.0f;
            float startRotation = 0.0f;
            cards::CardSet startSet = { cards::CardSets::NONE, -1 };
            bool moving = false;
            float progress = 0.0f;
        };
        std::vector<_Card> _cards;
        cards::Card* _hoveredCard = nullptr;

        static constexpr float CARD_WIDTH = 160;
        static constexpr float CARD_HEIGHT = 250;

        static constexpr float PI = 3.141592f;
        static constexpr float RADIAN = 57.2958f;

        ID2D1Bitmap* _offenseCardBitmap = nullptr;
        ID2D1Bitmap* _defenseCardBitmap = nullptr;
        ID2D1Bitmap* _utilityCardBitmap = nullptr;
        ID2D1Bitmap* _comboCardBitmap = nullptr;
        float shadowRadius = 2.0f;

    protected:
        friend class Scene;
        friend class Base;
        Board(Scene* scene) : Base(scene) {}
        void Init(Core* core)
        {
            this->core = core;

            _CreateMissingCards();
        }
    public:
        ~Board() {}
        Board(Board&&) = delete;
        Board& operator=(Board&&) = delete;
        Board(const Board&) = delete;
        Board& operator=(const Board&) = delete;

    private:

        void _GenerateCardBitmap(Graphics g, ID2D1Bitmap** bitmapRef, D2D1_COLOR_F color);
        void _CreateMissingCards();
        void _CalculateCardTargetPositions();
        cards::Card* _GetHoveredCard();
    };
}