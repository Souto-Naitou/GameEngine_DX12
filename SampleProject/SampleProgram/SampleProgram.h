#pragma once

#include <Framework/NimaFramework.h>
#include <Features/TimeMeasurer/TimeMeasurer.h>
#include <Features/Model/Loader/IModelLoader.h>
#include <Features/Model/ModelStorage.h>
#include <Features/Model/ModelManager.h>

class SampleProgram : public NimaFramework
{
public:
    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize() override;

    /// <summary>
    /// 終了
    /// </summary>
    void Finalize() override;

    /// <summary>
    /// 更新
    /// </summary>
    void Update() override;

    /// <summary>
    /// 描画
    /// </summary>
    void Draw() override;

    /// <summary>
    /// プログラム終了判定
    /// </summary>
    bool IsExitProgram() { return isExitProgram_; }


private: /// ゲーム内オブジェクト
    std::unique_ptr<IModelLoader> pModelLoader_ = nullptr; // !< モデルローダー
    std::unique_ptr<ModelStorage> pModelStorage_ = nullptr; // !< モデルストレージ
    std::unique_ptr<ModelManager> pModelManager_ = nullptr; // !< モデルマネージャー
};