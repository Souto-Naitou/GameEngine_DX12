#include "GameScene.h"

#include <Features/Object3d/Object3dSystem.h>
#include <Features/Particle/ParticleSystem.h>
#include <Features/SceneManager/SceneManager.h>
#include <Features/Line/LineSystem.h>
#include <Core/Win32/WinSystem.h>
#include <Features/GameEye/FreeLook/FreeLookEye.h>
#include <NiGui/NiGui.h>
#include <Features/Audio/AudioManager.h>


void GameScene::Initialize()
{
    pInput_ = Input::GetInstance();

    //pFirework_ = std::make_unique<ParticleEmitter>();
    //pSmoke_ = std::make_unique<ParticleEmitter>();
    //pSpark_ = std::make_unique<ParticleEmitter>();


    pGameEye_ = std::make_unique<FreeLookEye>();
    pSkydome_ = std::make_unique<Object3d>();
    pGrid_ = std::make_unique<Object3d>();

    pGuideSprite_ = std::make_unique<Sprite>();
    pGuideSprite_->Initialize("Text/SceneChangeGuide.png");
    pGuideSprite_->SetName("GuideText");
    pGuideSprite_->SetPosition(Vector2(WinSystem::clientWidth - 40.0f, WinSystem::clientHeight - 40.0f));
    pGuideSprite_->SetAnchorPoint({ 1,1 });

    pGameEye_->SetRotate({ 0.1f, 0.0f, 0.0f });
    pGameEye_->SetTranslate({ 0.0f, 0.2f, -5.0f });
    pGameEye_->SetName("MainCamera");

    /// システムにデフォルトのゲームカメラを設定
    Object3dSystem::GetInstance()->SetSharedGameEye(pGameEye_.get());
    ParticleSystem::GetInstance()->SetDefaultGameEye(pGameEye_.get());
    LineSystem::GetInstance()->SetDefaultGameEye(pGameEye_.get());


    pSkydome_->Initialize("Skydome.obj");
    pSkydome_->SetScale({ 1.0f, 1.0f, 1.0f });
    pSkydome_->SetName("Skydome");
    pSkydome_->SetEnableLighting(false);

    pGrid_->Initialize("Grid_v4.obj");
    pGrid_->SetScale({ 1.0f, 1.0f, 1.0f });
    pGrid_->SetName("Grid");
    pGrid_->SetTilingMultiply({ 100.0f, 100.0f });
    pGrid_->SetEnableLighting(false);

    /// エミッタの初期化
    //pFirework_->Initialize("Box/Box.obj", "Resources/Json/Box.json");
    //pSmoke_->Initialize("Particle/ParticleSpark.obj", "Resources/Json/Smoke.json");
    //pSpark_->Initialize("Particle/ParticleSpark.obj", "Resources/Json/Spark.json");

    /// 音声の取得
    pAudio_ = AudioManager::GetInstance()->GetNewAudio("pi.wav");
}

void GameScene::Finalize()
{
    pSkydome_->Finalize();
    pGrid_->Finalize();
    pGuideSprite_->Finalize();
    pAudio_->Finalize();
    //pFirework_->Finalize();
    //pSmoke_->Finalize();
    //pSpark_->Finalize();
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
    //pFirework_->Update();
    //pSmoke_->Update();
    //pSpark_->Update();

    auto center = NiGui_StandardPoint::Center;

    NiGui::BeginDivMovable("Div1", "white1x1.png", NiGui::YELLOW, { 0, -100 }, { 200, 200 }, center, center);
    if (NiGui::Button("Button1", "white1x1.png", NiGui::BLUE, { 0, 0 }, { 100, 100 }, {}, center, center) == NiGui_ButtonState::Confirm)
    {
        /// Do anything...
    }
    NiGui::EndDiv();

}

void GameScene::Draw2dBackGround()
{
}

void GameScene::Draw3dMidground()
{
}

void GameScene::Draw2dMidground()
{
}

void GameScene::DrawLine()
{
    //pFirework_->Draw();
    //pSmoke_->Draw();
    //pSpark_->Draw();
}

void GameScene::Draw3d()
{
    pSkydome_->Draw();
    pGrid_->Draw();
}

void GameScene::Draw2dForeground()
{
    pGuideSprite_->Draw();
}

void GameScene::DrawTexts()
{
}
