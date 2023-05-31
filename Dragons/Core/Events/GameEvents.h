#pragma once

#include <vector>
#include <mutex>
#include <any>

template<class _Event>
class EventSubscriber;

class GameEvents
{
private:
    template<class _Event>
    friend class EventSubscriber;

    struct _EventData
    {
        uint64_t eventsRaised;
        const char* eventName;
        std::vector<std::any> subscribers;
    };
    std::vector<_EventData> _events;

    template<class _Event>
    void _Subscribe(EventSubscriber<_Event>* sub);

    template<class _Event>
    void _Unsubscribe(EventSubscriber<_Event>* sub);

public:
    template<class _Event>
    void RaiseEvent(_Event ev);
};

#include "EventSubscriber.h"

template<class _Event>
void GameEvents::_Subscribe(EventSubscriber<_Event>* sub)
{
    if (!sub)
        return;

    for (_EventData& eventData : _events)
    {
        if (eventData.eventName == _Event::_NAME_())
        {
            auto subscriber = std::find_if(
                eventData.subscribers.begin(),
                eventData.subscribers.end(),
                [&](std::any& value) { return std::any_cast<EventSubscriber<_Event>*>(value) == sub; }
            );
            if (subscriber == eventData.subscribers.end())
            {
                eventData.subscribers.push_back(std::any(sub));
            }
            return;
        }
    }
    _events.push_back({ 0, _Event::_NAME_(), { std::any(sub) } });
}

template<class _Event>
void GameEvents::_Unsubscribe(EventSubscriber<_Event>* sub)
{
    if (!sub)
        return;

    for (auto eventIt = _events.begin(); eventIt != _events.end(); eventIt++)
    {
        _EventData& eventData = *eventIt;
        if (eventData.eventName == _Event::_NAME_())
        {
            auto subscriber = std::find_if(
                eventData.subscribers.begin(),
                eventData.subscribers.end(),
                [&](std::any& value) { return std::any_cast<EventSubscriber<_Event>*>(value) == sub; }
            );
            if (subscriber != eventData.subscribers.end())
            {
                eventData.subscribers.erase(subscriber);
                if (eventData.subscribers.empty())
                {
                    _events.erase(eventIt);
                }
            }
            return;
        }
    }
}

template<class _Event>
void GameEvents::RaiseEvent(_Event ev)
{
    for (int i = 0; i < _events.size(); i++)
    {
        if (_events[i].eventName == _Event::_NAME_())
        {
            // Send event to subscribers
            for (int j = 0; j < _events[i].subscribers.size(); j++)
            {
                EventSubscriber<_Event>* subscriber = std::any_cast<EventSubscriber<_Event>*>(_events[i].subscribers[j]);
                if (!subscriber)
                {
                    // This should NEVER be hit
                    throw std::exception("Non-matching event subscriber type. Likely two events with the same name exist");
                }
                subscriber->_OnEvent(ev);
            }

            // Move often called events closer to vector start
            _events[i].eventsRaised++;
            if (i > 0 && _events[i].eventsRaised > _events[i - 1].eventsRaised)
                std::swap(_events[i], _events[i - 1]);

            break;
        }
    }
}