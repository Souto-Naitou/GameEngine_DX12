#pragma once

#include <Features/Audio/Audio.h>
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

// ゲーム共有
class NimaFramework
{
public:
    void Run();


public:
    virtual ~NimaFramework() {}

    virtual void Initialize();
    virtual void Finalize();
    virtual void Update();
    virtual void Draw() = 0;

    virtual bool IsExitProgram() const { return isExitProgram_; }


protected: /// システムクラスのインスタンス
    ISceneFactory* pSceneFactory_ = nullptr;
    ImGuiManager* pImGuiManager_;
    Audio* pAudio_;


protected: /// 他クラスのインスタンス
    DirectX12* pDirectX_;
    DebugManager* pDebugManager_;
    WinSystem* pWinSystem_;
    ModelManager* pModelManager_;
    SRVManager* pSRVManager_;
    TextureManager* pTextureManager_;
    SceneManager* pSceneManager_;
    SpriteSystem* pSpriteSystem_;
    Object3dSystem* pObject3dSystem_;
    ParticleSystem* pParticleSystem_;
    ParticleManager* pParticleManager_;
    LineSystem* pLineSystem_;
    Input* pInput_;


protected:
    bool isExitProgram_ = false;
};