#pragma once

#include <Features/Input/Input.h>
#include <Interfaces/IScene.h>
#include <Features/Object3d/Object3d.h>
#include <Features/GameEye/GameEye.h>


#include <memory>
#include <Features/Particle/Emitter/ParticleEmitter.h>
#include <string>

class CG4Task1 : public IScene
{
public:
    /// Common function
    void    Initialize() override;
    void    Finalize() override;
    void    Update() override;
    void    Draw3dMidground() override;
    void    Draw2dMidground() override;
    void    DrawLine() override;
    void    Draw3d() override;
    void    Draw2dBackGround() override;
    void    Draw2dForeground() override;
    void    DrawTexts() override;

private:
    // Resources
    std::unique_ptr<GameEye>            pGameEye_           = nullptr;
    std::unique_ptr<Object3d>           pGrid_              = nullptr;
    std::unique_ptr<ParticleEmitter>    pEmitter_Basic_     = nullptr;
    std::unique_ptr<ParticleEmitter>    pEmitter_Stars_     = nullptr;
    std::unique_ptr<ParticleEmitter>    pEmitter_Rain_      = nullptr;
    std::unique_ptr<ParticleEmitter>    pEmitter_Snow_      = nullptr;
    std::unique_ptr<ParticleEmitter>    pEmitter_Spark_     = nullptr;
    std::unique_ptr<ParticleEmitter>    pEmitter_Test_      = nullptr;

    // Pointers
    Input*  pInput_     = nullptr;
};