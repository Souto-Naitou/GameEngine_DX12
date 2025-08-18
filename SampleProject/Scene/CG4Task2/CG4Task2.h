#pragma once

#include <Features/Input/Input.h>
#include <Scene/SceneBase.h>
#include <Features/Object3d/Object3d.h>
#include <Features/GameEye/GameEye.h>


#include <memory>
#include <Features/Text/Text.h>
#include <Features/Model/IModel.h>
#include <Features/Model/ModelManager.h>
#include <Features/Model/GltfModelSystem.h>
#include <Features/Line/LineSystem.h>

class CG4Task2 : public SceneBase
{
public:
    /// Common function
    CG4Task2(ISceneArgs* pArgs_) : SceneBase(pArgs_) {}
    void    Initialize() override;
    void    Finalize() override;
    void    Update() override;
    void    Draw() override;
    void    DrawTexts() override;

private:
    // Resources
    std::unique_ptr<GameEye>            pGameEye_           = nullptr;
    std::unique_ptr<Object3d>           pGrid_              = nullptr;
    std::unique_ptr<Object3d>           pSimple_            = nullptr;
    std::unique_ptr<IModel>             pModelSimple_       = nullptr;
    std::unique_ptr<IModel>             pModelGrid_         = nullptr;
    std::unique_ptr<Text>               pText_              = nullptr;

    // Pointers
    Input*  pInput_     = nullptr;
    ModelManager* pModelManager_ = nullptr;
    GltfModelSystem* pGltfModelSystem_ = nullptr;
    LineSystem* pLineSystem_ = nullptr;

    void UpdateController();
};