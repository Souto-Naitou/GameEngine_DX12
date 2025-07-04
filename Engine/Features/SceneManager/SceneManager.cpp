#include "SceneManager.h"

#include <imgui.h>
#include <DebugTools/DebugManager/DebugManager.h>
#include <Core/ConfigManager/ConfigManager.h>

#include <cassert>

void SceneManager::ReserveScene(const std::string& _name)
{
    isReserveScene_ = true;
    nextSceneName_ = _name;

    return;
}

void SceneManager::ReserveStartupScene()
{
    auto& cfgData = ConfigManager::GetInstance()->GetConfigData();
    this->ReserveScene(cfgData.start_scene);
}

void SceneManager::Initialize()
{
    DebugManager::GetInstance()->SetComponent("Core", name_, std::bind(&SceneManager::DebugWindow, this), true);
    ReserveStartupScene();
}

void SceneManager::Update()
{
    if (!pSceneTransitionManager_) pSceneTransitionManager_ = SceneTransitionManager::GetInstance();

    if (isReserveScene_)
    {
        ChangeScene();
        isReserveScene_ = false;
    }

    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Update();
    }

    pSceneTransitionManager_->Update();
}

void SceneManager::SceneDraw2dBackGround()
{
    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Draw2dBackGround();
    }
}

void SceneManager::SceneDraw3d()
{
    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Draw3d();
    }
}

void SceneManager::SceneDraw2dMidground()
{
    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Draw2dMidground();
    }
}

void SceneManager::SceneDraw3dMidground()
{
    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Draw3dMidground();
    }
}

void SceneManager::SceneDrawLine()
{
    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->DrawLine();
    }
}

void SceneManager::SceneDraw2dForeground()
{
    pSceneTransitionManager_->Draw();

    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Draw2dForeground();
    }
}

void SceneManager::SceneDrawText()
{
    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->DrawTexts();
    }
}

void SceneManager::Finalize()
{
    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Finalize();
    }

    DebugManager::GetInstance()->DeleteComponent("#Window", name_.c_str());
}

void SceneManager::ChangeScene()
{
    assert(pSceneFactory_);

    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Finalize();
    }

    pCurrentScene_ = pSceneFactory_->CreateScene(nextSceneName_);
    pCurrentScene_->Initialize();
}

void SceneManager::DebugWindow()
{
#ifdef _DEBUG

    ImGui::InputText("Next Scene Name", buffer, 128);
    ImGui::SameLine();
    if (ImGui::Button("Change"))
    {
        this->ReserveScene(buffer);
    }

#endif // _DEBUG
}