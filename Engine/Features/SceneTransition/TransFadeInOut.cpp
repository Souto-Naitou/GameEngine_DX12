#include "TransFadeInOut.h"
#include <Core/DirectX12/TextureManager.h>
#include <DebugTools/DebugManager/DebugManager.h>
#include <DebugTools/ImGuiTemplates/ImGuiTemplates.h>
#include <Features/SceneManager/SceneManager.h>
#include <Core/Win32/WinSystem.h>
#include <MathExtension/mathExtension.h>

void TransFadeInOut::Initialize(const std::string& _sceneName)
{
    sceneName_ = _sceneName;

    screenWidth_ = WinSystem::clientWidth;
    screenHeight_ = WinSystem::clientHeight;

    sprite_ = std::make_unique<Sprite>();
    sprite_->Initialize("white1x1.png");
    sprite_->SetColor({ 0,0,0,0 });
    sprite_->SetSize({ screenWidth_, screenHeight_ });
    timer_.Start();
    DebugManager::GetInstance()->SetComponent(
        "Transition", name_, std::bind(&TransFadeInOut::DebugWindow, this));

    name_ = "FadeInOut";
}

void TransFadeInOut::Update()
{
    if (countPhase_ == 2)
    {
        isEnd_ = true;
        return;
    }
    if (timer_.GetNow<double>() > duration_)
    {
        timer_.Reset();
        timer_.Start();
        countPhase_++;
    }
    if (!isChangedScene_ && countPhase_ == 1)
    {
        isChangedScene_ = true;
        SceneManager::GetInstance()->ReserveScene(sceneName_);
    }

    if (countPhase_ == 0)
    {
        opacity_ = Math::Lerp(0.0f, 1.0f, static_cast<float>(timer_.GetNow<double>() / duration_));
    }
    else if (countPhase_ == 1)
    {
        opacity_ = Math::Lerp(1.0f, 0.0f, static_cast<float>(timer_.GetNow<double>() / duration_));
    }

    sprite_->SetColor(Vector4(0, 0, 0, opacity_));
    sprite_->Update();
}

void TransFadeInOut::Draw()
{
    sprite_->Draw();
}

void TransFadeInOut::Finalize()
{
    UnregisterDebugWindowC("Transition", name_);
    sprite_->Finalize();
}

void TransFadeInOut::DebugWindow()
{
#ifdef _DEBUG

    auto pFunc = [&]()
    {
        ImGuiTemplate::VariableTableRow("Scene Name", sceneName_);
        ImGuiTemplate::VariableTableRow("Timer", timer_.GetNow<double>());
        ImGuiTemplate::VariableTableRow("Phase", countPhase_);
        ImGuiTemplate::VariableTableRow("Opacity", opacity_);
    };

    ImGuiTemplate::VariableTable("SCT_FadeInOut", pFunc);

#endif
}

TransFadeInOut::~TransFadeInOut()
{
    OutputDebugStringA("deleted TransFadeInOut\n");
}
