#include "SceneManager.h"

#include <imgui.h>
#include <DebugTools/DebugManager/DebugManager.h>
#include <Core/ConfigManager/ConfigManager.h>
#include <Scene/Args/SceneArgs.h>

#include <cassert>

void SceneManager::SetSceneFactory(ISceneFactory* _pSceneFactory)
{
    pSceneFactory_ = _pSceneFactory;
}

void SceneManager::SetSceneArgs(std::unique_ptr<ISceneArgs> _pSceneArgs)
{
    pSceneArgs_ = std::move(_pSceneArgs);
}

void SceneManager::SetModelManager(ModelManager* _pModelManager)
{
    pModelManager_ = _pModelManager;
}

SceneManager& SceneManager::AddInitialArg(const std::string& _key, const std::any& _value)
{
    initialArgs_[_key] = _value;
    return *this;
}

void SceneManager::ReserveScene(const std::string& _name)
{
    isReserveScene_ = true;
    nextSceneName_ = _name;
}

void SceneManager::ReserveStartupScene()
{
    auto& cfgData = ConfigManager::GetInstance()->GetConfigData();
    this->ReserveScene(cfgData.start_scene);
}

void SceneManager::Initialize()
{
    DebugManager::GetInstance()->SetComponent("Core", name_, std::bind(&SceneManager::DebugWindow, this), true);

    pSceneArgs_ = std::make_unique<SceneArgs>();

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

void SceneManager::SceneDraw()
{
    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Draw();
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

    DebugManager::GetInstance()->DeleteComponent("Core", name_);
}

void SceneManager::ChangeScene()
{
    assert(pSceneFactory_);

    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Finalize();
    }

    this->PackSceneArgs();

    pCurrentScene_ = pSceneFactory_->CreateScene(nextSceneName_, pSceneArgs_.get());
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

void SceneManager::PackSceneArgs()
{
    if (pSceneArgs_ == nullptr)
    {
        pSceneArgs_ = std::make_unique<SceneArgs>();
    }

    for (const auto& [key, value] : initialArgs_)
    {
        pSceneArgs_->Set(key, value);
    }

    pSceneArgs_->Set("ModelManager", pModelManager_);
}
