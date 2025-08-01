#include "SceneFactory.h"

#include <Scene/GameScene/GameScene.h>
#include <Scene/InstancingScene/InstancingScene.h>
#include <Scene/CG3PT2/CG3PT2.h>
#include <Scene/DepthScene/DepthScene.h>
#include <Scene/PG3PT1/PG3PT1.h>
#include <Scene/CG4Task1/CG4Task1.h>
#include <Scene/CG4Task2/CG4Task2.h>
#include <Scene/CG4Task3/CG4Task3.h>
#include <Scene/CG5Task1/CG5Task1.h>
#include <Scene/TL1Task1/TL1Task1.h>

#include <cassert>

#define JUDGE_SCENE(class, arg) if (_sceneName == #class) { return std::make_unique<class>(arg); }

std::unique_ptr<SceneBase> SceneFactory::CreateScene(const std::string & _sceneName, ISceneArgs* _pArgs)
{
    JUDGE_SCENE(InstancingScene, _pArgs)
    else JUDGE_SCENE(GameScene, _pArgs)
    else JUDGE_SCENE(CG3PT2, _pArgs)
    else JUDGE_SCENE(DepthScene, _pArgs)
    else JUDGE_SCENE(PG3PT1, _pArgs)
    else JUDGE_SCENE(CG4Task1, _pArgs)
    else JUDGE_SCENE(CG4Task2, _pArgs)
    else JUDGE_SCENE(CG5Task1, _pArgs)
    else JUDGE_SCENE(TL1Task1, _pArgs)
    else JUDGE_SCENE(CG4Task3, _pArgs)

    assert(false && "シーンの生成に失敗しました");

    return nullptr;
}
