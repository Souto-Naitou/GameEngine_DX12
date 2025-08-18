#include "CG5Task1.h"
#include <Features/Model/ObjModel.h>
#include <Features/GameEye/FreeLook/FreeLookEye.h>
#include <Features/Particle/ParticleSystem.h>
#include <Presets/Object3d/Grid/Preset_Grid.h>

void CG5Task1::Initialize()
{
    pLogger_ = Logger::GetInstance();
    pTextureManager_ = TextureManager::GetInstance();

    // 引数の分解
    this->_DecomposeArg();

    // カメラの初期化
    pGameEye_ = std::make_unique<FreeLookEye>();
    pGameEye_->SetRotate({ 0.0f, 0.0f, 0.0f });
    pGameEye_->SetTranslate({ -5.6f, 4.0f, -28.0f });
    pGameEye_->SetName("MainCamera");

    // システムにデフォルトのゲームカメラを設定 (カメラ生成後に実行)
    Object3dSystem::GetInstance()->SetGlobalEye(pGameEye_.get());
    ParticleSystem::GetInstance()->SetGlobalEye(pGameEye_.get());

    // ポストエフェクトの初期化
    this->_InitializePostEffects();

    // モデルの初期化
    this->_InitializeModels();

    // パーティクルの初期化 (Modelに依存)
    this->_InitializeParticles();

    // Object3dの初期化 (Modelに依存)
    this->_InitializeObject3d();

    // ディゾルブにテクスチャを設定
    pPEDissolve_->SetTextureResource(pTextureManager_->GetTextureResource("noise0.png"));

    // 計測スタート
    globalTimer_.Start();
}

// 終了処理
void CG5Task1::Finalize()
{
    pGrid_->Finalize();
    pShelf_->Finalize();
    pBench_->Finalize();
    pStreetLight_->Finalize();

    pModelGrid_->Finalize();
    pModelSpark_->Finalize();
    pModelShelf_->Finalize();
    pModelBench_->Finalize();
    pModelStreetLight_->Finalize();

    pEmitter_Spark_->Finalize();
    pEmitter_Firework_->Finalize();
    pEmitter_Test1_->Finalize();

    // パーティクルの終了処理
    this->_FinalizePostEffects();
}


// 更新処理
void CG5Task1::Update()
{
    pGameEye_->Update();
    pPERandomFilter_->SetSeed(globalTimer_.GetNow<float>() * 0.01f);

    pModelGrid_->Update();
    pModelSpark_->Update();
    pModelShelf_->Update();
    pModelBench_->Update();
    pModelStreetLight_->Update();
    pGrid_->Update();
    pShelf_->Update();
    pBench_->Update();
    pStreetLight_->Update();
    pEmitter_Spark_->Update();
    pEmitter_Firework_->Update();
    pEmitter_Test1_->Update();
}


// 描画処理
void CG5Task1::Draw()
{
    pGrid_->Draw();
    pShelf_->Draw();
    pBench_->Draw();
    pStreetLight_->Draw();
}

// テキストの描画
void CG5Task1::DrawTexts()
{
}





/// ポストエフェクトの初期化
void CG5Task1::_InitializePostEffects()
{
    pPEGrayscale_               = Helper::PostEffect::CreatePostEffect<Grayscale>(pDirectX_);
    pPEVignette_                = Helper::PostEffect::CreatePostEffect<Vignette>(pDirectX_);
    pPEBoxFilter_               = Helper::PostEffect::CreatePostEffect<BoxFilter>(pDirectX_);
    pPEGaussianFilter_          = Helper::PostEffect::CreatePostEffect<GaussianFilter>(pDirectX_);
    pPEPrewittOutline_          = Helper::PostEffect::CreatePostEffect<PrewittOutline>(pDirectX_);
    pPEDepthBasedOutline_       = Helper::PostEffect::CreatePostEffect<DepthBasedOutline>(pDirectX_);
    pPERadialBlur_              = Helper::PostEffect::CreatePostEffect<RadialBlur>(pDirectX_);
    pPEDissolve_                = Helper::PostEffect::CreatePostEffect<Dissolve>(pDirectX_);
    pPERandomFilter_            = Helper::PostEffect::CreatePostEffect<RandomFilter>(pDirectX_);
    pPESeparatedGaussianFilter_ = Helper::PostEffect::CreatePostEffect<SeparatedGaussianFilter>(pDirectX_);
    pPEGaussianBloom_           = Helper::PostEffect::CreatePostEffect<GaussianBloom>(pDirectX_);
    pPELuminanceOutput_         = Helper::PostEffect::CreatePostEffect<LuminanceOutput>(pDirectX_);

    (*pPostEffectExecuter_)
        .RegisterPostEffect(pPEGrayscale_.get())
        .RegisterPostEffect(pPEVignette_.get())
        .RegisterPostEffect(pPEBoxFilter_.get())
        .RegisterPostEffect(pPEGaussianFilter_.get())
        .RegisterPostEffect(pPEPrewittOutline_.get())
        .RegisterPostEffect(pPEDepthBasedOutline_.get())
        .RegisterPostEffect(pPERadialBlur_.get())
        .RegisterPostEffect(pPEDissolve_.get())
        .RegisterPostEffect(pPERandomFilter_.get())
        .RegisterPostEffect(pPESeparatedGaussianFilter_.get())
        .RegisterPostEffect(pPEGaussianBloom_.get())
        .RegisterPostEffect(pPELuminanceOutput_.get());

    // GaussianBloom
    auto& optLuminance = pPEGaussianBloom_->GetLuminanceOutputFilter()->GetOption();
    auto& optGaussian = pPEGaussianBloom_->GetSeparatedGaussianFilter()->GetOption();
    auto& optBloom = pPEGaussianBloom_->GetOption();
    optGaussian.kernelSize = 21;
    optLuminance.threshold = 0.5f;
    optBloom.bloomIntensity = 2.0f;
    pPEGaussianBloom_->GetSeparatedGaussianFilter()->SetSigma(7.5f);

    // DepthOutline
    auto& optDepthOutline = pPEDepthBasedOutline_->GetOption();
    optDepthOutline.weightMultiply = 1.0f;
}

/// ポストエフェクトの終了処理
void CG5Task1::_FinalizePostEffects()
{
    (*pPostEffectExecuter_)
        .UnregisterPostEffect(pPEGrayscale_.get())
        .UnregisterPostEffect(pPEVignette_.get())
        .UnregisterPostEffect(pPEBoxFilter_.get())
        .UnregisterPostEffect(pPEGaussianFilter_.get())
        .UnregisterPostEffect(pPEPrewittOutline_.get())
        .UnregisterPostEffect(pPEDepthBasedOutline_.get())
        .UnregisterPostEffect(pPERadialBlur_.get())
        .UnregisterPostEffect(pPEDissolve_.get())
        .UnregisterPostEffect(pPERandomFilter_.get())
        .UnregisterPostEffect(pPESeparatedGaussianFilter_.get())
        .UnregisterPostEffect(pPEGaussianBloom_.get())
        .UnregisterPostEffect(pPELuminanceOutput_.get());
}

/// 引数の分解
void CG5Task1::_DecomposeArg()
{
    try
    {
        pDirectX_ = std::any_cast<DirectX12*>(pArgs_->Get("DirectX12"));
        pPostEffectExecuter_ = std::any_cast<PostEffectExecuter*>(pArgs_->Get("PostEffectExecuter"));
        pModelManager_ = std::any_cast<ModelManager*>(pArgs_->Get("ModelManager"));
    }
    catch (const std::runtime_error& e)
    {
        pLogger_->LogError(__FILE__, __FUNCTION__, e.what());
        assert(0);
    }
}

/// パーティクルの初期化
void CG5Task1::_InitializeParticles()
{
    pEmitter_Spark_ = std::make_unique<ParticleEmitter>();
    pEmitter_Spark_->Initialize(pModelSpark_.get(), "Resources/Json/Spark.json");
    pEmitter_Spark_->SetEnableBillboard(true);

    pEmitter_Firework_ = std::make_unique<ParticleEmitter>();
    pEmitter_Firework_->Initialize(pModelSpark_.get(), "Resources/Json/Firework.json");
    pEmitter_Firework_->SetEnableBillboard(true);

    pEmitter_Test1_ = std::make_unique<ParticleEmitter>();
    pEmitter_Test1_->Initialize(pModelSpark_.get());
    pEmitter_Test1_->SetEnableBillboard(true);
}

/// モデルの初期化
void CG5Task1::_InitializeModels()
{
    // グリッドの初期化
    pModelGrid_ = std::make_unique<ObjModel>();
    pModelGrid_->Clone(pModelManager_->Load("Grid_v4/Grid_v4.obj"));

    // スパークの初期化
    pModelSpark_ = std::make_unique<ObjModel>();
    pModelSpark_->Clone(pModelManager_->Load("Particle/ParticleSpark.obj"));

    // L-Shelfの初期化
    pModelShelf_ = std::make_unique<ObjModel>();
    pModelShelf_->Clone(pModelManager_->Load("L-Shelf/L-Shelf.obj"));

    // Benchの初期化
    pModelBench_ = std::make_unique<ObjModel>();
    pModelBench_->Clone(pModelManager_->Load("Bench/Bench_HighRes.obj"));

    // StreetLightの初期化
    pModelStreetLight_ = std::make_unique<ObjModel>();
    pModelStreetLight_->Clone(pModelManager_->Load("StreetLight/StreetLight_HighRes.obj"));
}

/// Object3dの初期化
void CG5Task1::_InitializeObject3d()
{
    pGrid_ = presets::grid::Create(pModelGrid_.get());

    pShelf_ = std::make_unique<Object3d>();
    pShelf_->Initialize();
    pShelf_->SetScale({ 10.0f, 7.0f, 3.0f });
    pShelf_->SetRotate({ 0.0f, -3.14f, 0.0f });
    pShelf_->SetTranslate({ -6.26f, 0.0f, 8.81f });
    pShelf_->SetName("Shelf");
    pShelf_->SetModel(pModelShelf_.get());

    pBench_ = std::make_unique<Object3d>();
    pBench_->Initialize();
    pBench_->SetScale({ 1.0f, 1.0f, 1.0f });
    pBench_->SetName("Bench");
    pBench_->SetModel(pModelBench_.get());
    pBench_->SetRotate({ 0.0f, -3.38f, 0.0f });
    pBench_->SetTranslate({ -7.83f, 0.34f, 8.52f });

    pStreetLight_ = std::make_unique<Object3d>();
    pStreetLight_->Initialize();
    pStreetLight_->SetScale({ 1.0f, 1.0f, 1.0f });
    pStreetLight_->SetRotate({ 0.0f, -2.2f, 0.0f });
    pStreetLight_->SetName("StreetLight");
    pStreetLight_->SetModel(pModelStreetLight_.get());
    pStreetLight_->SetTranslate({ -4.28f, 0.33f, 8.55f });
}
