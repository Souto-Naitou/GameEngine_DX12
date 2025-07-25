#include "GameScene.h"

#include <Features/Object3d/Object3dSystem.h>
#include <Features/Particle/ParticleSystem.h>
#include <Features/SceneManager/SceneManager.h>
#include <Features/Line/LineSystem.h>
#include <Core/Win32/WinSystem.h>
#include <Features/GameEye/FreeLook/FreeLookEye.h>
#include <NiGui/NiGui.h>
#include <Features/Audio/AudioManager.h>
#include <Features/Model/ModelManager.h>


void GameScene::Initialize()
{
    pInput_ = Input::GetInstance();

    // モデルマネージャーの取得
    pModelManager_ = std::any_cast<ModelManager*>(pArgs_->Get("ModelManager"));

    // 各種モデルの取得
    pModelSkydome_  = pModelManager_->Load("Skydome.obj");
    pModelGrid_     = pModelManager_->Load("Grid_v4.obj");
    pModelSpark_    = pModelManager_->Load("Particle/ParticleSpark.obj");
    pModelBox_      = pModelManager_->Load("Box/Box.obj");

    // ゲームカメラの生成と初期化
    pGameEye_ = std::make_unique<FreeLookEye>();
    pGameEye_->SetRotate({ 0.1f, 0.0f, 0.0f });
    pGameEye_->SetTranslate({ 0.0f, 0.2f, -5.0f });
    pGameEye_->SetName("MainCamera");

    /// システムにデフォルトのゲームカメラを設定
    Object3dSystem::GetInstance()->SetGlobalEye(pGameEye_.get());
    ParticleSystem::GetInstance()->SetGlobalEye(pGameEye_.get());
    LineSystem::GetInstance()->SetGlobalEye(pGameEye_.get());

    // 各種オブジェクトの生成と初期化
    pSkydome_ = std::make_unique<Object3d>();
    pSkydome_->Initialize();
    pSkydome_->SetScale({ 1.0f, 1.0f, 1.0f });
    pSkydome_->SetName("Skydome");
    pSkydome_->SetEnableLighting(false);
    pSkydome_->SetModel(pModelSkydome_);

    pGrid_ = std::make_unique<Object3d>();
    pGrid_->Initialize();
    pGrid_->SetScale({ 1.0f, 1.0f, 1.0f });
    pGrid_->SetName("Grid");
    pGrid_->SetTilingMultiply({ 100.0f, 100.0f });
    pGrid_->SetEnableLighting(false);
    pGrid_->SetModel(pModelGrid_);

    // ガイド用のスプライトを生成
    pGuideSprite_ = std::make_unique<Sprite>();
    pGuideSprite_->Initialize("Text/SceneChangeGuide.png");
    pGuideSprite_->SetName("GuideText");
    pGuideSprite_->SetPosition(Vector2(WinSystem::clientWidth - 40.0f, WinSystem::clientHeight - 40.0f));
    pGuideSprite_->SetAnchorPoint({ 1,1 });

    // パーティクルエミッターの生成
    pFirework_ = std::make_unique<ParticleEmitter>();
    pFirework_->Initialize(pModelBox_, "Resources/Json/Box.json");
    pSmoke_ = std::make_unique<ParticleEmitter>();
    pSmoke_->Initialize(pModelSpark_, "Resources/Json/Smoke.json");
    pSpark_ = std::make_unique<ParticleEmitter>();
    pSpark_->Initialize(pModelSpark_, "Resources/Json/Spark.json");

    /// 音声の取得
    pAudio_ = AudioManager::GetInstance()->GetNewAudio("pi.wav");
}

void GameScene::Finalize()
{
    pSkydome_->Finalize();
    pGrid_->Finalize();
    pGuideSprite_->Finalize();
    pFirework_->Finalize();
    pSmoke_->Finalize();
    pSpark_->Finalize();
    pAudio_->Finalize();
}

void GameScene::Update()
{
    if (pInput_->PushKey(DIK_LCONTROL) && pInput_->PushKey(DIK_1))
    {
        SceneManager::GetInstance()->ReserveScene("InstancingScene");
    }

    /// 更新処理
    pGameEye_->Update();
    pGuideSprite_->Update();
    pSkydome_->Update();
    pGrid_->Update();
    pFirework_->Update();
    pSmoke_->Update();
    pSpark_->Update();
}

void GameScene::Draw()
{
    // =============================================
    // [Object3d Begin]
    pSkydome_->Draw();
    pGrid_->Draw();
    // [Object3d End]
    // =============================================

    // =============================================
    // [Particle Begin]
    pFirework_->Draw();
    // [Particle End]
    // =============================================

    // =============================================
    // [Sprite Begin]
    pGuideSprite_->Draw();
    // [Sprite End]
    // =============================================
}

void GameScene::DrawTexts()
{
}
