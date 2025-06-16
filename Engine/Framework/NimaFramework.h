#pragma once

#include <Core/ConfigManager/ConfigManager.h>
#include <DebugTools/Logger/Logger.h>
#include <Features/Audio/AudioManager.h>
#include <Features/Input/Input.h>
#include <Core/DirectX12/TextureManager.h>
#include <DebugTools/DebugManager/DebugManager.h>
#include <DebugTools/ImGuiManager/ImGuiManager.h>
#include <Features/Sprite/SpriteSystem.h>
#include <Features/Object3d/Object3dSystem.h>
#include <Features/Particle/ParticleSystem.h>
#include <Features/Model/ModelManager.h>
#include <Core/DirectX12/DirectX12.h>
#include <Core/Win32/WinSystem.h>
#include <Core/DirectX12/SRVManager.h>
#include <Features/SceneManager/SceneManager.h>
#include <Interfaces/ISceneFactory.h>
#include <Features/Particle/ParticleManager.h>
#include <Features/Line/LineSystem.h>
#include <Features/RandomGenerator/RandomGenerator.h>
#include <Features/Text/TextSystem.h>
#include <Features/Viewport/Viewport.h>
#include <Features/NiGui/NiGuiDrawer.h>
#include <Features/NiGui/NiGuiDebug.h>
#include <DebugTools/EventTimer/EventTimer.h>
#include <Effects/PostEffects/Grayscale/Grayscale.h>
#include <Effects/PostEffects/Vignette/Vignette.h>
#include <Effects/PostEffects/BoxFilter/BoxFilter.h>

#include <memory> /// std::unique_ptr
#include <Core/DirectX12/PostEffect.h>


/// ゲーム共通のフレームワーククラス
class NimaFramework
{
public:
    void                            Run();


public:
    virtual                         ~NimaFramework() {}

    virtual void                    Initialize();
    virtual void                    Finalize();
    virtual void                    Update();
    virtual void                    Draw();
    virtual bool                    IsExitProgram() const { return isExitProgram_; }

    void                            PreProcess();
    void                            PostProcess();


protected: 
    /// システムクラスのインスタンス
    std::unique_ptr<ISceneFactory>  pSceneFactory_              = nullptr;
    std::unique_ptr<Viewport>       pViewport_                  = nullptr;
    std::unique_ptr<NiGuiDrawer>    pDrawer_                    = nullptr;
    std::unique_ptr<NiGuiDebug>     pNiGuiDebug_                = nullptr;

    #ifdef _DEBUG
    std::unique_ptr<ImGuiManager>   pImGuiManager_              = nullptr;
    #endif // _DEBUG

    /// 他クラスのインスタンス
    ConfigManager*                  pConfigManager_             = nullptr;
    Logger*                         pLogger_                    = nullptr;
    DirectX12*                      pDirectX_                   = nullptr;
    DebugManager*                   pDebugManager_              = nullptr;
    WinSystem*                      pWinSystem_                 = nullptr;
    ModelManager*                   pModelManager_              = nullptr;
    SRVManager*                     pSRVManager_                = nullptr;
    TextureManager*                 pTextureManager_            = nullptr;
    SceneManager*                   pSceneManager_              = nullptr;
    SpriteSystem*                   pSpriteSystem_              = nullptr;
    Object3dSystem*                 pObject3dSystem_            = nullptr;
    ParticleSystem*                 pParticleSystem_            = nullptr;
    ParticleManager*                pParticleManager_           = nullptr;
    LineSystem*                     pLineSystem_                = nullptr;
    TextSystem*                     pTextSystem_                = nullptr;
    Input*                          pInput_                     = nullptr;
    RandomGenerator*                pRandomGenerator_           = nullptr;
    AudioManager*                   pAudioManager_              = nullptr;
    EventTimer*                     pEventTimer_                = nullptr;
    PostEffectExecuter*                     pPostEffect_                = nullptr;

    // ポストエフェクト
    std::unique_ptr<Grayscale>      pPEGrayscale_       = nullptr;
    std::unique_ptr<Vignette>       pPEVignette_        = nullptr;
    std::unique_ptr<BoxFilter>      pPEBoxFilter_       = nullptr;


protected:
    bool                            isExitProgram_              = false;
};



#define CREATE_APPLICATION(class) \
int _stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) \
{ \
    std::unique_ptr<NimaFramework> program = std::make_unique<class>(); \
    program->Run(); \
    return 0; \
}

#define CREATE_FRAMEWORK(classname) \
\
class classname : public NimaFramework \
{ \
public: \
    void Initialize() override; \
    void Finalize() override; \
    void Update() override; \
    void Draw() override; \
    bool IsExitProgram() { return isExitProgram_; } \
private: \
};