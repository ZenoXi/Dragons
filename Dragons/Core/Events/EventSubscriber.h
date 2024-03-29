#pragma once

class GameEvents;

template<class _Event>
class EventSubscriber
{
    friend class GameEvents;

    GameEvents* _eventClass;

    virtual void _OnEvent(_Event) = 0;
public:
    EventSubscriber(GameEvents* eventClass, bool maxPriority = false);
    EventSubscriber(const EventSubscriber<_Event>&) = delete;
    virtual ~EventSubscriber();
};

#include "GameEvents.h"

template<class _Event>
EventSubscriber<_Event>::EventSubscriber(GameEvents* eventClass, bool maxPriority) : _eventClass(eventClass)
{
    _eventClass->_Subscribe<_Event>(this, maxPriority);
}

template<class _Event>
EventSubscriber<_Event>::~EventSubscriber()
{
    _eventClass->_Unsubscribe<_Event>(this);
}

#include <functional>

template<class _Event>
class EventHandler : public EventSubscriber<_Event>
{
private:
    std::function<void(_Event)> _handler;

    void _OnEvent(_Event ev)
    {
        _handler(ev);
    }
public:
    EventHandler(GameEvents* eventClass, std::function<void(_Event)> handler, bool maxPriority = false)
        : EventSubscriber<_Event>(eventClass, maxPriority)
        , _handler(handler)
    {}

    void SetHandler(std::function<void(_Event)> handler)
    {
        _handler = handler;
    }
};