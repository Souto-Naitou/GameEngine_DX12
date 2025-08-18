#include "CG4Task1.h"

#include <Features/Object3d/Object3dSystem.h>
#include <Features/Particle/ParticleSystem.h>
#include <Features/Particle/ParticleManager.h>
#include <Features/Line/LineSystem.h>
#include <Features/GameEye/FreeLook/FreeLookEye.h>

#include <DebugTools/DebugManager/DebugManager.h>
#include <Core/DirectX12/TextureManager.h>
#include <Features/Model/ObjModel.h>
#include <Presets/Object3d/Grid/Preset_Grid.h>


void CG4Task1::Initialize()
{
    pInput_ = Input::GetInstance();
    pModelManager_ = std::any_cast<ModelManager*>(pArgs_->Get("ModelManager"));

    pGameEye_ = std::make_unique<FreeLookEye>();
    pGameEye_->SetRotate({ 0.1f, 0.0f, 0.0f });
    pGameEye_->SetTranslate({ 0.0f, 0.2f, -5.0f });
    pGameEye_->SetName("MainCamera");

    /// システムにデフォルトのゲームカメラを設定
    Object3dSystem::GetInstance()->SetGlobalEye(pGameEye_.get());
    ParticleSystem::GetInstance()->SetGlobalEye(pGameEye_.get());
    LineSystem::GetInstance()->SetGlobalEye(pGameEye_.get());

    // グリッドモデルのクローン
    pModelGrid_ = std::make_unique<ObjModel>();
    pModelGrid_->Clone(pModelManager_->Load("Grid_v4/Grid_v4.obj"));

    // グリッドの初期化
    pGrid_ = presets::grid::Create(pModelGrid_.get());

    // テクスチャの読み込み
    auto tm = TextureManager::GetInstance();
    tm->LoadTexture("circle2.png");
    tm->LoadTexture("spark.png");

    // パーティクルとモデルの初期化
    auto pSrc = pModelManager_->Load("Particle/ParticleSpark.obj");
    pModelSpark_ = std::make_unique<ObjModel>();
    pModelCircle_ = std::make_unique<ObjModel>();
    pModelSpark_->Clone(pSrc);
    pModelCircle_->Clone(pSrc);

    pModelSpark_->ChangeTexture(tm->GetSrvHandleGPU("spark.png"));
    pModelCircle_->ChangeTexture(tm->GetSrvHandleGPU("circle2.png"));

    pEmitter_Basic_ = std::make_unique<ParticleEmitter>();
    pEmitter_Basic_->Initialize(pModelCircle_.get(), "Resources/Json/CG4Task.json");
    pEmitter_Basic_->SetEnableBillboard(false);

    pEmitter_Stars_ = std::make_unique<ParticleEmitter>();
    pEmitter_Stars_->Initialize(pModelCircle_.get(), "Resources/Json/Stars.json");
    pEmitter_Stars_->SetEnableBillboard(true);

    pEmitter_Rain_ = std::make_unique<ParticleEmitter>();
    pEmitter_Rain_->Initialize(pModelSpark_.get(), "Resources/Json/Rain.json");
    pEmitter_Rain_->SetEnableBillboard(true);
    
    pEmitter_Snow_ = std::make_unique<ParticleEmitter>();
    pEmitter_Snow_->Initialize(pModelSpark_.get(), "Resources/Json/Snow.json");
    pEmitter_Snow_->SetEnableBillboard(true);

    pEmitter_Spark_ = std::make_unique<ParticleEmitter>();
    pEmitter_Spark_->Initialize(pModelSpark_.get(), "Resources/Json/Spark.json");
    pEmitter_Spark_->SetEnableBillboard(true);

    pEmitter_Test_ = std::make_unique<ParticleEmitter>();
    pEmitter_Test_->Initialize(pModelSpark_.get());
    pEmitter_Test_->SetEnableBillboard(true);

    pText_ = std::make_unique<Text>();
    pText_->Initialize();
    pText_->SetName("FPSTEXT");
    pText_->SetFontFamily("Bahnschrift");
    pText_->SetFontSize(12);
    pText_->SetColorName("White");
    pText_->SetPosition({ 10.0f, 10.0f });
    pText_->SetText("FPS: 0.0");
    pText_->SetMaxSize({ 200.0f, 50.0f });
}

void CG4Task1::Finalize()
{
    pText_->Finalize();
    ParticleManager::GetInstance()->ReleaseAllParticle();
    pEmitter_Test_->Finalize();
    pEmitter_Spark_->Finalize();
    pEmitter_Snow_->Finalize();
    pEmitter_Rain_->Finalize();
    pEmitter_Stars_->Finalize();
    pEmitter_Basic_->Finalize();
    pGrid_->Finalize();
}

void CG4Task1::Update()
{
    /// 更新処理
    pGameEye_->Update();
    pModelGrid_->Update();
    pModelSpark_->Update();
    pModelCircle_->Update();
    pGrid_->Update();
    pEmitter_Basic_->Update();
    pEmitter_Stars_->Update();
    pEmitter_Rain_->Update();
    pEmitter_Snow_->Update();
    pEmitter_Spark_->Update();
    pEmitter_Test_->Update();
    pText_->SetText("FPS: " + std::to_string(static_cast<int>(DebugManager::GetInstance()->GetFPS())));
    pText_->Update();
}

void CG4Task1::Draw()
{    
    // =============================================
    // [DrawLine Begin]
    pEmitter_Test_->Draw();
    // [DrawLine End]
    // =============================================

    // =============================================
    // [Object3d Begin]
    pGrid_->Draw();
    // [Object3d End]
    // =============================================
}

void CG4Task1::DrawTexts()
{
    pText_->Draw();
}
