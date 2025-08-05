#include "TL1Task1.h"

#include <Features/LevelLoader/LevelHelper.h>
#include <imgui.h>
#include <DebugTools/DebugManager/DebugManager.h>

void TL1Task1::Initialize()
{
    pModelManager_ = std::any_cast<ModelManager*>(pArgs_->Get("ModelManager"));

    directionalLight_.color = { 1.0f, 1.0f, 1.0f, 1.0f };       // 白色光
    directionalLight_.direction = { 0.0f, -1.0f, 0.0f };        // 真上から下
    directionalLight_.intensity = 0.4f;                         // 多少強めに

    sceneObjects_ = Helper::Level::LoadScene("Resources/Json/Levels/test.json", pModelManager_);
    sceneObjects_.SetDirectionalLight(&directionalLight_);
    DebugManager::GetInstance()->SetComponent("TL1Task1", std::bind(&TL1Task1::ImGui, this));
}

void TL1Task1::Finalize()
{
    DebugManager::GetInstance()->DeleteComponent("TL1Task1");
    sceneObjects_.Finalize();
    pModelManager_ = nullptr;
}

void TL1Task1::Update()
{
    sceneObjects_.Update();
}

void TL1Task1::Draw()
{
    sceneObjects_.Draw();
}

void TL1Task1::DrawTexts()
{
}

void TL1Task1::ImGui()
{
    #ifdef _DEBUG

    if (ImGui::Button("Reload Level"))
    {
        this->_ReloadLevel();
    }

    #endif // _DEBUG
}

void TL1Task1::_LoadLevel()
{
    sceneObjects_ = Helper::Level::LoadScene("Resources/Json/Levels/test.json", pModelManager_);
    sceneObjects_.SetDirectionalLight(&directionalLight_);
}

void TL1Task1::_ReloadLevel()
{
    sceneObjects_.Finalize();
    Helper::Level::Unload("Resources/Json/Levels/test.json");
    this->_LoadLevel();
}
