#include "SampleProgram.h"

#include <Features/SceneManager/SceneManager.h>
#include <Scene/Factory/SceneFactory.h>
#include <Features/Model/Loader/Assimp/ModelLoaderAssimp.h>
#include <Features/Model/Loader/ModelLoaderLegacy.h>


void SampleProgram::Initialize()
{
    /// 基底クラスの初期化処理
    NimaFramework::Initialize();

    #ifdef _DEBUG
    pImGuiManager_->EnableDocking();
    //pImGuiManager_->EnableMultiViewport();
    //pImGuiManager_->StyleMaterialFlat();
    #endif // _DEBUG
    
    /// シーンファクトリの設定
    pSceneFactory_ = std::make_unique<SceneFactory>();
    pSceneManager_->SetSceneFactory(pSceneFactory_.get());

    pModelLoader_ = std::make_unique<ModelLoaderAssimp>();
    pModelLoader_->Initialize();
    pModelLoader_->SetDirectX12(pDirectX_.get());
    pModelStorage_ = std::make_unique<ModelStorage>();

    pModelManager_ = std::make_unique<ModelManager>();
    pModelManager_->Initialize();
    pModelManager_->SetModelLoader(pModelLoader_.get());
    pModelManager_->SetModelStorage(pModelStorage_.get());

    pSceneManager_->SetModelManager(pModelManager_.get());

    /// モデルを全てロード
    pTextureManager_->LoadTexture("noise0.png");
    pPEDissolve_->SetTextureResource(pTextureManager_->GetTextureResource("noise0.png"));

    // 計測スタート
    globalTimer_.Start();
}

void SampleProgram::Finalize()
{
    /// 基底クラスの終了処理
    NimaFramework::Finalize();
    pModelStorage_->Finalize();
}

void SampleProgram::Update()
{
    /// 基底クラスの更新処理
    NimaFramework::Update();
    pModelLoader_->Update();
    pPERandomFilter_->SetSeed(globalTimer_.GetNow<float>() * 0.01f);
}

void SampleProgram::Draw()
{
    /// 描画前処理
    NimaFramework::PreProcess();


    /// バックバッファ書き込み
    NimaFramework::Draw();


    /// 描画後処理
    NimaFramework::PostProcess();
}
