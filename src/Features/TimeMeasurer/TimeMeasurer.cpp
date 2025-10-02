#include "./TimeMeasurer.h"

TimeMeasurer::TimeMeasurer()
{
    QueryPerformanceFrequency(&mFreq_);
}

void TimeMeasurer::Start()
{
    if (isRunning_) return;

    QueryPerformanceCounter(&mStart_);
    isStart_ = true;
    isRunning_ = true;
}

void TimeMeasurer::Stop()
{
    if (!isRunning_) return;

    GetNow<double>();
    nowBeforeStop_ += now_;
    isRunning_ = false;
}

void TimeMeasurer::Reset()
{
    mStart_ = {};
    now_ = 0.0;
    nowBeforeStop_ = 0.0;
    isStart_ = false;
    isRunning_ = false;
}