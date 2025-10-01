#pragma once

#include <Scene/SceneBase.h>
#include <Features/Object3d/Object3d.h>
#include <Features/GameEye/GameEye.h>
#include <Features/Text/Text.h>
#include <Features/Audio/Audio.h>

#include <Common/structs.h>

#include <memory>
#include <string>
#include <Features/Model/ModelManager.h>

class CG3PT2 : public SceneBase
{
public:
    CG3PT2(ISceneArgs* pArgs_) : SceneBase(pArgs_) {}
    ~CG3PT2() = default;

    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;
    void DrawTexts() override;


private:
    std::unique_ptr<GameEye>    pGameEye_               = nullptr;

    DirectionalLight            directionalLight_       = {};
    PointLight                  pointLight_             = {};

    IModel*                     pModelMonsterBall_      = nullptr;
    IModel*                     pModelGrid_             = nullptr;
    std::unique_ptr<Object3d>   pMonsterBall_           = nullptr;
    std::unique_ptr<Object3d>   pGrid_                  = nullptr;

    std::unique_ptr<Text>       pText_                  = nullptr;

    std::string                 name_                   = "CG3";

    D3D12_VIEWPORT              viewport_               = {};


private:
    void DebugWindow();

    ModelManager* pModelManager_ = nullptr;
};
