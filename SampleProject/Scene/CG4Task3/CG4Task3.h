#pragma once

#include <Scene/SceneBase.h>
#include <memory>
#include <Features/Model/ModelManager.h>
#include <Features/Model/GltfModelSystem.h>
#include <Features/Line/LineSystem.h>
#include <Features/Cubemap/CubemapSystem.h>
#include <Features/Object3d/Object3d.h>
#include <Features/Model/IModel.h>
#include <Features/GameEye/GameEye.h>
#include <Features/Cubemap/Skybox.h>

class CG4Task3 : public SceneBase
{
public:
    CG4Task3(ISceneArgs* _args) : SceneBase(_args) {};
    ~CG4Task3() override = default;

    void Initialize() override;

    void Finalize() override;

    void Update() override;

    void Draw() override;

    void DrawTexts() override;

private:
    void _ImGui();
    void _ReloadTexture();

    // For Debug
    const std::string name_ = "CG4Task3";
    bool requestReloadTexture_ = false;

    // Resources
    std::unique_ptr<GameEye>            pGameEye_           = nullptr;
    std::unique_ptr<Object3d>           pGrid_              = nullptr;
    std::unique_ptr<Object3d>           pSimple_            = nullptr;
    std::unique_ptr<IModel>             pModelSimple_       = nullptr;
    std::unique_ptr<IModel>             pModelGrid_         = nullptr;
    std::unique_ptr<Skybox>             pSkybox_            = nullptr;

    // Pointers
    ModelManager* pModelManager_ = nullptr;
    GltfModelSystem* pGltfModelSystem_ = nullptr;
    LineSystem* pLineSystem_ = nullptr;
    CubemapSystem* pCubemapSystem_ = nullptr;
};