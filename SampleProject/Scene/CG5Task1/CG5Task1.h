#pragma once

// Scene
#include <Scene/SceneBase.h>
#include <Interfaces/ISceneArgs.h>

// PostEffects
#include <Effects/PostEffects/Grayscale/Grayscale.h>
#include <Effects/PostEffects/Vignette/Vignette.h>
#include <Effects/PostEffects/BoxFilter/BoxFilter.h>
#include <Effects/PostEffects/GaussianFilter/GaussianFilter.h>
#include <Effects/PostEffects/PrewittOutline/PrewittOutline.h>
#include <Effects/PostEffects/DepthBasedOutline/DepthBasedOutline.h>
#include <Effects/PostEffects/RadialBlur/RadialBlur.h>
#include <Effects/PostEffects/Dissolve/Dissolve.h>
#include <Effects/PostEffects/RandomFilter/RandomFilter.h>
#include <Effects/PostEffects/SeparatedGaussianFilter/SeparatedGaussianFilter.h>
#include <Effects/PostEffects/GaussianBloom/GaussianBloom.h>

// Engine classes
#include <Core/DirectX12/PostEffect.h>
#include <DebugTools/Logger/Logger.h>
#include <Core/DirectX12/TextureManager.h>
#include <Features/Model/ModelManager.h>
#include <Features/Object3d/Object3d.h>
#include <Features/Model/IModel.h>
#include <Features/Particle/Emitter/ParticleEmitter.h>

class CG5Task1 : public SceneBase
{
public:
    CG5Task1(ISceneArgs* pArgs_) : SceneBase(pArgs_) {}
    void Initialize()   override;
    void Finalize()     override;
    void Update()       override;
    void Draw()         override;
    void DrawTexts()    override;

private:
    DirectX12*                                  pDirectX_                   = nullptr;
    Logger*                                     pLogger_                    = nullptr;
    PostEffectExecuter*                         pPostEffectExecuter_        = nullptr;
    TextureManager*                             pTextureManager_            = nullptr;
    ModelManager*                               pModelManager_              = nullptr;

    // ポストエフェクト
    std::unique_ptr<Grayscale>                  pPEGrayscale_               = nullptr;
    std::unique_ptr<Vignette>                   pPEVignette_                = nullptr;
    std::unique_ptr<BoxFilter>                  pPEBoxFilter_               = nullptr;
    std::unique_ptr<GaussianFilter>             pPEGaussianFilter_          = nullptr;
    std::unique_ptr<PrewittOutline>             pPEPrewittOutline_          = nullptr;
    std::unique_ptr<DepthBasedOutline>          pPEDepthBasedOutline_       = nullptr;
    std::unique_ptr<RadialBlur>                 pPERadialBlur_              = nullptr;
    std::unique_ptr<Dissolve>                   pPEDissolve_                = nullptr;
    std::unique_ptr<RandomFilter>               pPERandomFilter_            = nullptr;
    std::unique_ptr<SeparatedGaussianFilter>    pPESeparatedGaussianFilter_ = nullptr;
    std::unique_ptr<GaussianBloom>              pPEGaussianBloom_           = nullptr;
    std::unique_ptr<LuminanceOutput>            pPELuminanceOutput_         = nullptr;

    TimeMeasurer                                globalTimer_;     // !< グローバルタイマー
    std::unique_ptr<GameEye>                    pGameEye_           = nullptr;

    // オブジェクト
    std::unique_ptr<Object3d>                   pGrid_              = nullptr;
    std::unique_ptr<Object3d>                   pShelf_             = nullptr;
    std::unique_ptr<Object3d>                   pBench_             = nullptr;
    std::unique_ptr<Object3d>                   pStreetLight_       = nullptr;

    // モデル
    std::unique_ptr<IModel>                     pModelGrid_         = nullptr;
    std::unique_ptr<IModel>                     pModelSpark_        = nullptr;
    std::unique_ptr<IModel>                     pModelShelf_        = nullptr;
    std::unique_ptr<IModel>                     pModelBench_        = nullptr;
    std::unique_ptr<IModel>                     pModelStreetLight_  = nullptr;

    // パーティクル
    std::unique_ptr<ParticleEmitter>            pEmitter_Spark_     = nullptr;
    std::unique_ptr<ParticleEmitter>            pEmitter_Firework_  = nullptr;
    std::unique_ptr<ParticleEmitter>            pEmitter_Test1_     = nullptr;


    void    _InitializePostEffects();
    void    _FinalizePostEffects();
    void    _DecomposeArg();
    void    _InitializeParticles();
    void    _InitializeModels();
    void    _InitializeObject3d();
};