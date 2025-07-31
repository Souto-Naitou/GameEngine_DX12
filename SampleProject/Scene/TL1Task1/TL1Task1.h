#pragma once

#include <Scene/SceneBase.h>
#include <Features/LevelLoader/SceneObjects.h>
#include <Interfaces/ISceneArgs.h>
#include <memory>
#include <Features/GameEye/GameEye.h>

class TL1Task1 : public SceneBase
{
public:
    TL1Task1(ISceneArgs* _pArgs) : SceneBase(_pArgs) {}

    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;
    void DrawTexts() override;

private:
    void _LoadLevel();
    void _ReloadLevel();
    void ImGui();

    SceneObjects sceneObjects_ = {};
    std::unique_ptr<GameEye> pGameEye_ = nullptr;
    ModelManager* pModelManager_ = nullptr;
    DirectionalLight directionalLight_ = {};
};