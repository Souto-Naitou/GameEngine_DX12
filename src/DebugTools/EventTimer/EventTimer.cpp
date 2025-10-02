#include "EventTimer.h"

#include <imgui.h>
#include <format>
#include <utility>

#undef min
#undef max

void EventTimer::NewFrame()
{
    globalTimer_.Start();
}

void EventTimer::BeginEvent(const std::string& _eventName)
{
    timers_[_eventName].Reset();
    timers_[_eventName].Start();
}

void EventTimer::EndEvent(const std::string& _eventName)
{
    float now = timers_[_eventName].GetNow<float>();
    events_[_eventName] = now;
    if (maxTime_[_eventName] < now)
    {
        maxTime_[_eventName] = now;
        maxTimeResetTimers_[_eventName].Reset();
        maxTimeResetTimers_[_eventName].Start();
    }
    if (maxTimeResetTimers_[_eventName].GetNow<float>() > static_cast<float>(durationMaxTimeDisplay_))
    {
        maxTime_[_eventName] = 0.0f;
    }
}

void EventTimer::Reset()
{
    timers_.clear();
    events_.clear();
}

void EventTimer::Measure(const std::string& _eventName, const std::function<void()>& _func)
{
    BeginEvent(_eventName);
    _func();
    EndEvent(_eventName);
}

void EventTimer::EndFrame()
{
    deltaTime_ = globalTimer_.GetNow<float>();
    globalTimer_.Reset();
}

void EventTimer::ImGui()
{
    #ifdef _DEBUG

    ImGui::Begin("EventTimer");

    ImGui::Checkbox("Display Progress Bar", &isDisplayProgressBar_);
    ImGui::InputInt("Display duration MaxTime ", &durationMaxTimeDisplay_);

    if (isDisplayProgressBar_)
    {
        for (const auto& event : events_)
        {
            ImGui::Text("%s : %.0fms", event.first.c_str(), event.second * 1000.0f);
            ImGui::ProgressBar(event.second / deltaTime_, ImVec2(0.0f, 0.0f), ""); ImGui::SameLine();
            ImGui::Text("%.0fms", maxTime_[event.first] * 1000.0f);
        }
    }
    else
    {
        for (const auto& event : events_)
        {
            ImGui::Text("%s : %.0fms (max : %.0fms)", event.first.c_str(), event.second * 1000.0f, maxTime_[event.first] * 1000.0f);
        }
    }

    ImGui::End();

    #endif // _DEBUG
}
