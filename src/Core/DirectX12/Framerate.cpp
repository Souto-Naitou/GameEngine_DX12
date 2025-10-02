#include "Framerate.h"

#include <thread>
#include <timeapi.h>

#pragma comment(lib, "Winmm.lib")

void FrameRate::Initialize()
{
    timeBeginPeriod(1);
    reference_ = std::chrono::steady_clock::now();
}

void FrameRate::FixFramerate()
{
    const std::chrono::microseconds kMinTime(static_cast<uint64_t>(1e6 / 60.0f));
    const std::chrono::microseconds kMinCheckTime(static_cast<uint64_t>(1e6 / 65.0f));

    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

    if (elapsed < kMinCheckTime)
    {
        while (std::chrono::steady_clock::now() - reference_ < kMinTime)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }

    reference_ = std::chrono::steady_clock::now();
}

void FrameRate::MeasureFPS()
{
    if (!timer_.GetIsStart())
    {
        timer_.Start();
    }
    /// フレームレート計算
    if (timer_.GetNow<double>() - elapsedFrameCount_ >= intervalCalcurationFPS_)
    {
        fps_ = frameCount_ * 1.0 / (timer_.GetNow<double>() - elapsedFrameCount_);

        frameCount_ = 0;
        elapsedFrameCount_ = timer_.GetNow<double>();
    }
    frameCount_++;
}
