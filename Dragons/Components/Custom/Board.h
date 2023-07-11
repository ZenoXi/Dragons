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

        void _OnMouseEnterArea()
        {

        }

        void _OnMouseLeaveArea()
        {

        }

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

        float CARD_WIDTH = 200;
        float CARD_HEIGHT = 300;

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

        void _CreateMissingCards();
        void _CalculateCardTargetPositions();
    };
}