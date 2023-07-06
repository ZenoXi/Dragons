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

    struct _EventInProgress
    {
        const char* name;
        int subIndex;
        bool eventErased;
    };
    std::vector<_EventInProgress> _eventsInProgress;

    template<class _Event>
    void _Subscribe(EventSubscriber<_Event>* sub, bool maxPriority = false);

    template<class _Event>
    void _Unsubscribe(EventSubscriber<_Event>* sub);

public:
    template<class _Event>
    void RaiseEvent(_Event ev);
};

#include "EventSubscriber.h"

template<class _Event>
void GameEvents::_Subscribe(EventSubscriber<_Event>* sub, bool maxPriority)
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
                if (maxPriority)
                    // If an event is being processed, adding a max priority subscriber
                    // will cause the current handler to be called twice
                    eventData.subscribers.insert(eventData.subscribers.begin(), std::any(sub));
                else
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
            for (int i = 0; i < eventData.subscribers.size(); i++)
            {
                if (std::any_cast<EventSubscriber<_Event>*>(eventData.subscribers[i]) != sub)
                    continue;
                
                // If an event gets unsubscribed while it is being processed,
                // ensure all remaining subscribers receive the event
                for (auto& event : _eventsInProgress)
                {
                    if (event.name != _Event::_NAME_())
                        continue;

                    if (event.subIndex <= i)
                        event.subIndex--;
                }

                eventData.subscribers.erase(eventData.subscribers.begin() + i);
                if (eventData.subscribers.empty())
                {
                    _events.erase(eventIt);

                    // If the event is erased mid processing, notify dispatch to avoid bad memory access
                    for (auto& event : _eventsInProgress)
                    {
                        if (event.name == _Event::_NAME_())
                            event.eventErased = true;
                    }
                }

                break;
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
            _EventInProgress event;
            event.name = _Event::_NAME_();
            event.eventErased = false;

            _eventsInProgress.push_back(event);
            auto& eventInProgress = _eventsInProgress.back();

            // Send event to subscribers
            for (eventInProgress.subIndex = 0; eventInProgress.subIndex < _events[i].subscribers.size(); eventInProgress.subIndex++)
            {
                EventSubscriber<_Event>* subscriber = std::any_cast<EventSubscriber<_Event>*>(_events[i].subscribers[eventInProgress.subIndex]);
                if (!subscriber)
                {
                    _eventsInProgress.pop_back();
                    // This should NEVER be hit
                    throw std::exception("Non-matching event subscriber type. Likely two events with the same name exist");
                }
                subscriber->_OnEvent(ev);

                if (eventInProgress.eventErased)
                    break;
            }

            _eventsInProgress.pop_back();
            if (eventInProgress.eventErased)
            {
                eventInProgress.eventErased = false;
                break;
            }

            // Move often called events closer to vector start
            _events[i].eventsRaised++;
            if (i > 0 && _events[i].eventsRaised > _events[i - 1].eventsRaised)
                std::swap(_events[i], _events[i - 1]);

            break;
        }
    }
}