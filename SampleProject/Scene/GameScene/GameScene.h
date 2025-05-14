#pragma once

#include <Features/Input/Input.h>
#include <Interfaces/IScene.h>
#include <Features/Object3d/Object3d.h>
#include <Features/GameEye/GameEye.h>
#include <Features/Particle/Emitter/ParticleEmitter.h>
#include <Features/Sprite/Sprite.h>
#include <Features/Audio/Audio.h>

#include <memory>

class GameScene : public IScene
{
public:
    void Initialize() override;

    void Finalize() override;

    void Update() override;

    void Draw2dBackGround() override;

    void Draw3dMidground() override;

    void Draw2dMidground() override;

    void DrawLine() override;

    void Draw3d() override;

    void Draw2dForeground() override;

    void DrawTexts() override;

private:
    std::unique_ptr<GameEye> pGameEye_;
    //std::unique_ptr<ParticleEmitter> pSmoke_;
    //std::unique_ptr<ParticleEmitter> pFirework_;
    //std::unique_ptr<ParticleEmitter> pSpark_;
    std::unique_ptr<Object3d> pSkydome_;
    std::unique_ptr<Object3d> pGrid_;
    std::unique_ptr<Sprite> pGuideSprite_;
    Audio* pAudio_;


private:
    Input* pInput_;


private:
    void EmitterSetting();

};