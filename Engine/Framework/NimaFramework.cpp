#include "NimaFramework.h"
#include <clocale>

#include <NiGui/NiGui.h>

void NimaFramework::Run()
{
    setlocale(LC_ALL, "ja_JP.Utf-8");

    /// 初期化
    Initialize();

    while (true)
    {
        /// 更新
        Update();

        /// 終了判定
        if (IsExitProgram())
        {
            break;
        }

        /// 描画
        #ifdef _DEBUG
        Draw();
        #else
        DrawHighPerformance();
        #endif // _DEBUG
    }

    Finalize();

    return;
}

void NimaFramework::Initialize()
{
    /// システムクラスの初期化
    pLogger_ = Logger::GetInstance();
    pDirectX_ = DirectX12::GetInstance();

    pDebugManager_ = DebugManager::GetInstance();
    pWinSystem_ = WinSystem::GetInstance();
    pModelManager_ = ModelManager::GetInstance();
    pSpriteSystem_ = SpriteSystem::GetInstance();
    pObject3dSystem_ = Object3dSystem::GetInstance();
    pParticleSystem_ = ParticleSystem::GetInstance();
    pTextureManager_ = TextureManager::GetInstance();
    pSRVManager_ = SRVManager::GetInstance();
    pSceneManager_ = SceneManager::GetInstance();
    pParticleManager_ = ParticleManager::GetInstance();
    pLineSystem_ = LineSystem::GetInstance();
    pInput_ = Input::GetInstance();
    pRandomGenerator_ = RandomGenerator::GetInstance();
    pTextSystem_ = TextSystem::GetInstance();
    pAudioManager_ = AudioManager::GetInstance();
    pEventTimer_ = EventTimer::GetInstance();

    #ifdef _DEBUG
    pImGuiManager_ = std::make_unique<ImGuiManager>();
    #endif // _DEBUG

    /// ロガーの初期化
    pLogger_->Initialize();

    /// ウィンドウの初期化
    pWinSystem_->Initialize();
    pWinSystem_->ShowWnd();

    /// DirectX12の初期化
    pDirectX_->Initialize();

    /// SRVManagerの初期化
    pSRVManager_->Initialize(pDirectX_);

    /// ImGui基盤の初期化
    #ifdef _DEBUG
    pImGuiManager_->Initialize(pDirectX_);
    #endif // _DEBUG

    /// テクスチャマネージャの初期化
    pTextureManager_->Initialize(pSRVManager_);

    /// モデルマネージャの初期化
    pModelManager_->Initialize();

    /// スプライト基盤の初期化
    pSpriteSystem_->Initialize();

    /// 3Dオブジェクト基盤の初期化
    pObject3dSystem_->Initialize();

    /// パーティクル基盤の初期化
    pParticleSystem_->Initialize();

    /// ライン基盤の初期化
    pLineSystem_->Initialize();

    /// テキスト基盤の初期化
    pTextSystem_->Initialize();

    /// オーディオの初期化
    pAudioManager_->Initialize();

    /// 入力の初期化
    pInput_->Initialize(GetModuleHandleA(nullptr), pWinSystem_->GetHwnd());

    /// 乱数生成器の初期化
    pRandomGenerator_->Initialize();

    /// SRVが初期化された後に呼ぶ
    pDirectX_->CreateGameScreenResource();

    /// ビューポートの初期化
    pViewport_ = std::make_unique<Viewport>();
    pViewport_->Initialize();
    pTextSystem_->SetViewport(pViewport_.get());

    /// シーンマネージャの初期化
    pSceneManager_->Initialize();

    /// UIの初期化
    auto vp = pDirectX_->GetViewport();
    NiGui::Initialize({ vp.Width, vp.Height }, { vp.TopLeftX, vp.TopLeftY });
    NiGui::SetClientSize({WinSystem::clientWidth, WinSystem::clientHeight});
    

    NiGui::SetConfirmSound(pAudioManager_->GetNewAudio("ui_confirm.wav"));
    NiGui::SetHoverSound(pAudioManager_->GetNewAudio("ui_hover.wav"));

    /// Drawerの設定
    pDrawer_ = std::make_unique<NiGuiDrawer>();
    NiGui::SetDrawer(pDrawer_.get());

    /// デバッグUIの設定
    auto& io = NiGui::GetIO();
    auto& state = NiGui::GetState();
    auto& setting = NiGui::GetSetting();
    pNiGuiDebug_ = std::make_unique<NiGuiDebug>();
    pNiGuiDebug_->SetIO(&io);
    pNiGuiDebug_->SetState(&state);
    pNiGuiDebug_->SetSetting(&setting);
   
    NiGui::SetDebug(pNiGuiDebug_.get());

    /// ポストエフェクト
    pPostEffect_ = std::make_unique<PostEffect>();
    pPostEffect_->Initialize();

    /// コマンドリストを追加
    pDirectX_->AddCommandList(pObject3dSystem_->GetCommandList());
    pDirectX_->AddCommandList(pParticleSystem_->GetCommandList());
    pDirectX_->AddCommandList(pSpriteSystem_->GetCommandList());
    pDirectX_->AddCommandList(pPostEffect_->GetCommandList());
}

void NimaFramework::Finalize()
{
    pAudioManager_->Finalize();
    pWinSystem_->Finalize();
    pLogger_->Save();
    pSceneManager_->Finalize();

    #ifdef _DEBUG
    pImGuiManager_->Finalize();
    #endif // _DEBUG
}

void NimaFramework::Update()
{
    UINT msg = pWinSystem_->GetMsg();
    if (msg == WM_QUIT)
    {
        isExitProgram_ = true;
        return;
    }

    #ifdef _DEBUG
    if(pWinSystem_->IsResized())
    {
        pImGuiManager_->Resize();
    }
    #endif // _DEBUG

    #ifdef _DEBUG

    /// UIの更新
    NiGui::SetWindowInfo(
        { pViewport_->GetViewportSize().x, pViewport_->GetViewportSize().y },
        { pViewport_->GetViewportPos().x, pViewport_->GetViewportPos().y }
    );

    #endif // _DEBUG

    /// イベント計測開始
    #ifdef _DEBUG
    pEventTimer_->NewFrame();
    pEventTimer_->BeginEvent("Update");
    #endif // _DEBUG

    NiGui::BeginFrame();

    /// マネージャ更新
    pInput_->Update();
    pModelManager_->Update();
    pAudioManager_->Update();

    #ifdef _DEBUG
    pImGuiManager_->BeginFrame();
    #endif // _DEBUG

    pDebugManager_->Update();
    pDebugManager_->DrawUI();
    pViewport_->DrawWindow();
    pLogger_->DrawUI();

    /// シーン更新
    pSceneManager_->Update();


    /// イベント計測終了
    #ifdef _DEBUG
    pEventTimer_->EndEvent("Update");
    #endif // _DEBUG


    #ifdef _DEBUG
    NiGui::DrawDebug();
    #endif // _DEBUG

    /// パーティクル更新
    pParticleManager_->Update();
}

void NimaFramework::Draw()
{
    /// イベント計測開始
    pEventTimer_->BeginEvent("Draw");

    /// 3D描画
    pObject3dSystem_->DepthDrawSetting();
    pSceneManager_->SceneDraw3d();
    pObject3dSystem_->MainDrawSetting();
    pSceneManager_->SceneDraw3d();

    /// 中景3dオブジェクトの描画
    pObject3dSystem_->MainDrawSetting();
    pSceneManager_->SceneDraw3dMidground();

    /// ライン描画
    pLineSystem_->PresentDraw();
    pSceneManager_->SceneDrawLine();

    /// パーティクル描画
    pParticleSystem_->PresentDraw();
    pParticleManager_->Draw();

    /// 前景スプライトの描画
    pSpriteSystem_->PresentDraw();
    pSceneManager_->SceneDraw2dForeground();

    /// UIの描画
    pSpriteSystem_->PresentDraw();
    NiGui::DrawUI();

    /// レンダーターゲットの初期化
    pDirectX_->NewFrame();

    pPostEffect_->Draw();

    /// レンダーターゲットからビューポート用リソースにコピー
    pDirectX_->CopyFromRTV(pDirectX_->GetCommandListsLast());
    /// コンピュートシェーダーの実行
    pViewport_->Compute();

    /// イベント計測終了と出力
    pEventTimer_->EndEvent("Draw");
    pEventTimer_->EndFrame();
    pEventTimer_->ImGui();


    /// ImGuiの描画
    #ifdef _DEBUG
    pImGuiManager_->Render();
    pImGuiManager_->EndFrame();
    #endif // _DEBUG

    /// コマンドの実行
    pDirectX_->CommandExecute();

    /// テキストの描画
    pTextSystem_->PresentDraw();
    pSceneManager_->SceneDrawText();
    pTextSystem_->PostDraw();
}

void NimaFramework::DrawHighPerformance()
{
    /// 3D描画
    pSceneManager_->SceneDraw3d();
    pObject3dSystem_->DrawCall();

    /// ライン描画
    pLineSystem_->PresentDraw();
    pSceneManager_->SceneDrawLine();

    /// パーティクル描画
    pParticleManager_->Draw();
    pParticleSystem_->DrawCall();

    /// 前景スプライトの描画
    pSceneManager_->SceneDraw2dForeground();
    NiGui::DrawUI();
    pSpriteSystem_->DrawCall();

    /// レンダーターゲットの初期化
    pDirectX_->NewFrame();

    /// 同期
    pObject3dSystem_->Sync();
    pParticleSystem_->Sync();
    pSpriteSystem_->Sync();

    pPostEffect_->Draw();

    /// コマンドの実行
    pDirectX_->CommandExecute();


    /// テキストの描画
    pTextSystem_->PresentDraw();
    pSceneManager_->SceneDrawText();
    pTextSystem_->PostDraw();
}

void NimaFramework::PreProcess()
{
    pPostEffect_->NewFrame();
    pSRVManager_->SetDescriptorHeaps();

    #ifndef _DEBUG

    pObject3dSystem_->SetRTVHandle(pPostEffect_->GetRTVHandle());
    pSpriteSystem_->SetRTVHandle(pPostEffect_->GetRTVHandle());
    pParticleSystem_->SetRTVHandle(pPostEffect_->GetRTVHandle());

    #endif // _DEBUG
}

void NimaFramework::PostProcess()
{
    pDirectX_->DisplayFrame();
    pObject3dSystem_->PostDraw();
    pSpriteSystem_->PostDraw();
    pParticleSystem_->PostDraw();
    pPostEffect_->PostDraw();
}
