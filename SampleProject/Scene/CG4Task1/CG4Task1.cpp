#include "CG4Task1.h"

#include <Features/Object3d/Object3dSystem.h>
#include <Features/Particle/ParticleSystem.h>
#include <Features/SceneManager/SceneManager.h>
#include <Features/Line/LineSystem.h>
#include <Features/GameEye/FreeLook/FreeLookEye.h>
#include <Core/DirectX12/PostEffect.h>


void CG4Task1::Initialize()
{
    pInput_ = Input::GetInstance();

    pGameEye_ = std::make_unique<FreeLookEye>();
    pGameEye_->SetRotate({ 0.1f, 0.0f, 0.0f });
    pGameEye_->SetTranslate({ 0.0f, 0.2f, -5.0f });
    pGameEye_->SetName("MainCamera");

    /// システムにデフォルトのゲームカメラを設定
    Object3dSystem::GetInstance()->SetGlobalEye(pGameEye_.get());
    ParticleSystem::GetInstance()->SetGlobalEye(pGameEye_.get());
    LineSystem::GetInstance()->SetGlobalEye(pGameEye_.get());

    pGrid_ = std::make_unique<Object3d>();
    pGrid_->Initialize("Grid_v4.obj");
    pGrid_->SetScale({ 1.0f, 1.0f, 1.0f });
    pGrid_->SetName("Grid");
    pGrid_->SetTilingMultiply({ 100.0f, 100.0f });
    pGrid_->SetEnableLighting(false);
    pGrid_->SetColor({ 1.0f, 1.0f, 1.0f, 0.3f });

    pEmitter_Basic_ = std::make_unique<ParticleEmitter>();
    pEmitter_Basic_->Initialize("Particle/ParticleSpark.obj", "circle2.png", "Resources/Json/CG4Task.json");
    pEmitter_Basic_->SetEnableBillboard(false);

    pEmitter_Stars_ = std::make_unique<ParticleEmitter>();
    pEmitter_Stars_->Initialize("Particle/ParticleSpark.obj", "circle2.png", "Resources/Json/Stars.json");
    pEmitter_Stars_->SetEnableBillboard(true);

    pEmitter_Rain_ = std::make_unique<ParticleEmitter>();
    pEmitter_Rain_->Initialize("Particle/ParticleSpark.obj", "spark.png", "Resources/Json/Rain.json");
    pEmitter_Rain_->SetEnableBillboard(true);
    
    pEmitter_Snow_ = std::make_unique<ParticleEmitter>();
    pEmitter_Snow_->Initialize("Particle/ParticleSpark.obj", "spark.png", "Resources/Json/Snow.json");
    pEmitter_Snow_->SetEnableBillboard(true);

    pEmitter_Spark_ = std::make_unique<ParticleEmitter>();
    pEmitter_Spark_->Initialize("Particle/ParticleSpark.obj", "spark.png", "Resources/Json/Spark.json");
    pEmitter_Spark_->SetEnableBillboard(true);

    pEmitter_Test_ = std::make_unique<ParticleEmitter>();
    pEmitter_Test_->Initialize("Particle/ParticleSpark.obj", "spark.png");
    pEmitter_Test_->SetEnableBillboard(true);
}

void CG4Task1::Finalize()
{
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
    pGrid_->Update();
    pEmitter_Basic_->Update();
    pEmitter_Stars_->Update();
    pEmitter_Rain_->Update();
    pEmitter_Snow_->Update();
    pEmitter_Spark_->Update();
    pEmitter_Test_->Update();
}

void CG4Task1::Draw2dBackGround()
{
}

void CG4Task1::Draw3dMidground()
{
}

void CG4Task1::Draw2dMidground()
{
}

void CG4Task1::DrawLine()
{
    pEmitter_Test_->Draw();
}

void CG4Task1::Draw3d()
{    
    pGrid_->Draw();
}

void CG4Task1::Draw2dForeground()
{
}

void CG4Task1::DrawTexts()
{
}
