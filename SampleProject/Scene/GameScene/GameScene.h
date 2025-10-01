#pragma once

#include <Features/Input/Input.h>
#include <Scene/SceneBase.h>
#include <Features/Object3d/Object3d.h>
#include <Features/GameEye/GameEye.h>
#include <Features/Particle/Emitter/ParticleEmitter.h>
#include <Features/Sprite/Sprite.h>
#include <Features/Audio/Audio.h>

#include <memory>
#include <Features/Model/ModelManager.h>

class GameScene : public SceneBase
{
public:
    GameScene(ISceneArgs* pArgs_) : SceneBase(pArgs_) {}

    void Initialize() override;

    void Finalize() override;

    void Update() override;

    void Draw() override;

    void DrawTexts() override;

private:
    IModel* pModelSkydome_  = nullptr;
    IModel* pModelGrid_     = nullptr;
    IModel* pModelSpark_    = nullptr;
    IModel* pModelBox_      = nullptr;

    std::unique_ptr<GameEye> pGameEye_;
    std::unique_ptr<ParticleEmitter> pFirework_;
    std::unique_ptr<ParticleEmitter> pSmoke_;
    std::unique_ptr<ParticleEmitter> pSpark_;
    std::unique_ptr<Object3d> pSkydome_;
    std::unique_ptr<Object3d> pGrid_;
    std::unique_ptr<Sprite> pGuideSprite_;

    // Pointers
    Input* pInput_ = nullptr;
    ModelManager* pModelManager_ = nullptr;
};