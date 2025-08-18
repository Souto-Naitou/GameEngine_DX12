#include "CG4Task3.h"
#include <Features/GameEye/FreeLook/FreeLookEye.h>
#include <Features/Particle/ParticleSystem.h>
#include <Features/Model/GltfModel.h>
#include <Features/Model/ObjModel.h>
#include <Core/DirectX12/TextureManager.h>
#include <Features/Model/Helper/ModelHelper.h>
#include <imgui.h>
#include <DebugTools/DebugManager/DebugManager.h>
#include <Presets/Object3d/Grid/Preset_Grid.h>

void CG4Task3::Initialize()
{
    RegisterDebugWindowS(name_, CG4Task3::_ImGui, false);

    pModelManager_ = std::any_cast<ModelManager*>(pArgs_->Get("ModelManager"));
    pGltfModelSystem_ = std::any_cast<GltfModelSystem*>(pArgs_->Get("GltfModelSystem"));
    pLineSystem_ = std::any_cast<LineSystem*>(pArgs_->Get("LineSystem"));
    pCubemapSystem_ = std::any_cast<CubemapSystem*>(pArgs_->Get("CubemapSystem"));

    pGameEye_ = std::make_unique<FreeLookEye>();
    pGameEye_->SetRotate({ 0.1f, 0.0f, 0.0f });
    pGameEye_->SetTranslate({ 0.0f, 0.2f, -5.0f });
    pGameEye_->SetName("MainCamera");

    /// システムにデフォルトのゲームカメラを設定
    Object3dSystem::GetInstance()->SetGlobalEye(pGameEye_.get());
    ParticleSystem::GetInstance()->SetGlobalEye(pGameEye_.get());
    pLineSystem_->SetGlobalEye(pGameEye_.get());
    pCubemapSystem_->SetGlobalEye(pGameEye_.get());

    // モデルの初期化
    auto pSrc = pModelManager_->Load("Human/Walk.gltf");
    pModelSimple_ = std::make_unique<GltfModel>();
    pModelSimple_->Initialize();
    pModelSimple_->Clone(pSrc);

    // モデルの初期化
    pModelGrid_ = std::make_unique<ObjModel>();
    pModelGrid_->Clone(pModelManager_->Load("Grid_v4/Grid_v4.obj"));

    // グリッドの初期化
    pGrid_ = presets::grid::Create(pModelGrid_.get());

    pSimple_ = std::make_unique<Object3d>();
    pSimple_->Initialize();
    pSimple_->SetScale({ 1.0f, 1.0f, 1.0f });
    pSimple_->SetName("sneakWalk");
    pSimple_->SetModel(pModelSimple_.get());

    auto tm = TextureManager::GetInstance();
    tm->LoadTexture("rostock_laage_airport_4k.dds");
    auto envTex = tm->GetSrvHandleGPU("rostock_laage_airport_4k.dds");

    Object3dSystem::GetInstance()->SetEnvironmentTexture(envTex);

    pSkybox_ = std::make_unique<Skybox>();
    pSkybox_->Initialize(pCubemapSystem_);
    pSkybox_->SetSkyboxTexture(envTex);
}

void CG4Task3::Finalize()
{
    pSkybox_->Finalize();
    pSimple_->Finalize();
    pGrid_->Finalize();
    pModelSimple_->Finalize();
    pModelGrid_->Finalize();
    UnregisterDebugWindowS(name_);
}

void CG4Task3::Update()
{
    { // Skybox
        if (requestReloadTexture_)
        {
            _ReloadTexture();
            requestReloadTexture_ = false;
        }
        pSkybox_->Update();
    }
    pGameEye_->Update();
    pGrid_->Update();
    pSimple_->Update();
    pModelSimple_->Update();
    pModelGrid_->Update();

    pGltfModelSystem_->PrepareDispatch();
    Helper::Model::DispatchModel(pModelGrid_.get());
    Helper::Model::DispatchModel(pModelSimple_.get());
}

void CG4Task3::Draw()
{
    pGrid_->Draw();
    pSimple_->Draw();

    pCubemapSystem_->DrawSetting();
    pSkybox_->Draw();
}

void CG4Task3::DrawTexts()
{
}

void CG4Task3::_ImGui()
{
    #ifdef _DEBUG

    requestReloadTexture_ = ImGui::Button("Reload Texture");

    #endif // _DEBUG
}

void CG4Task3::_ReloadTexture()
{
    auto pTm = TextureManager::GetInstance();
    pTm->ReloadTexture("rostock_laage_airport_4k.dds");
    auto envTex = pTm->GetSrvHandleGPU("rostock_laage_airport_4k.dds");
    pSkybox_->SetSkyboxTexture(envTex);
    Object3dSystem::GetInstance()->SetEnvironmentTexture(envTex);
}
