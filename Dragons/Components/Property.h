#pragma once

#include "Helper/Time.h"

#include <functional>

namespace zcom
{
    template<typename T>
    class Property
    {
    public:
        enum InterpolationFunction
        {
            LINEAR,
            EASE_IN,
            EASE_OUT,
            EASE_IN_OUT
        };

        Property() {}
        Property(T value) { _value = value; }
        Property(const Property<T>& other) { other._CopyStateTo(*this); }
        Property<T>& operator=(const Property<T>& other) { other._CopyStateTo(*this); return *this; }

        void SetValue(T value)
        {
            if (_animateValue)
            {
                _animating = true;
                _animationStartTime = _Clock()->Now();
                _startValue = _value;
                _endValue = value;
            }
            else
            {
                _value = value;
            }
        }
        T GetValue()
        {
            if (_animating)
            {
                Duration timeElapsed = _Clock()->Now() - _animationStartTime;
                float delta = timeElapsed.GetTicks() / (float)_animationDuration.GetTicks();
                if (delta >= 1.0f)
                {
                    delta = 1.0f;
                    _animating = false;
                    _value = _endValue;
                }
                else
                {
                    delta = _interpolationFunction(delta);
                    _value = _startValue + (_endValue - _startValue) * delta;
                }
            }

            return _value;
        }

        void SetExternalClock(Clock* clock)
        {
            _externalClock = clock;
        }

        void EnableAnimation()
        {
            _animateValue = true;
        }
        void DisableAnimation()
        {
            _animateValue = false;
        }

        void SetAnimationDuration(Duration duration)
        {
            _animationDuration = duration;
        }

        void SetInterpolationFunction(std::function<float(float)> func)
        {
            _interpolationFunction = func;
        }
        void SetInterpolationFunction(InterpolationFunction functionPreset)
        {

        }

    private:
        void _CopyStateTo(Property<T>& other) const
        {
            other._value = _value;

            other._internalClock = _internalClock;
            other._externalClock = _externalClock;

            other._animateValue = _animateValue;
            other._animating = _animating;
            other._startValue = _startValue;
            other._endValue = _endValue;
            other._animationStartTime = _animationStartTime;
            other._animationDuration = _animationDuration;
            other._interpolationFunction = _interpolationFunction;
        }

        T _value{};

        Clock _internalClock = Clock(0);
        Clock* _externalClock = nullptr;
        Clock* _Clock()
        {
            if (_externalClock)
            {
                return _externalClock;
            }
            else
            {
                _internalClock.Update();
                return &_internalClock;
            }
        }

        // Animation
        bool _animateValue = false;
        bool _animating = false;
        T _startValue{};
        T _endValue{};
        TimePoint _animationStartTime = 0;
        Duration _animationDuration = 0;
        std::function<float(float)> _interpolationFunction = [](float delta) { return delta; }; // Linear interpolation by default
    };
}