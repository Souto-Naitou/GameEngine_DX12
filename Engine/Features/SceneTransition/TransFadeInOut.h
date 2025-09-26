#pragma once

#include "Features/SceneTransition/TransBase.h"
#include <string>
#include <Features/Sprite/Sprite.h>
#include <Features/TimeMeasurer/TimeMeasurer.h>


class TransFadeInOut : public TransBase
{
public:
    ~TransFadeInOut();
    void Initialize(const std::string& _sceneName) override;
    void Update() override;
    void Draw() override;
    void Finalize() override;
    void ImGui() override;

private:

    uint32_t screenWidth_ = 1280;
    uint32_t screenHeight_ = 720;

    std::string name_;

    float opacity_ = 0.0f;
    bool isChangedScene_ = false;

    double duration_ = 1.0;

    uint32_t countPhase_ = 0;

    std::string sceneName_;
    std::unique_ptr<Sprite> sprite_;
    std::unordered_map<std::string, TimeMeasurer> fadeTimers_;
};