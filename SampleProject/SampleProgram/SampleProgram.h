#pragma once

#include <Framework/NimaFramework.h>

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
    /// 高パフォーマンス描画 (Alpha)
    /// </summary>
    void DrawHighPerformance() override;

    /// <summary>
    /// プログラム終了判定
    /// </summary>
    bool IsExitProgram() { return isExitProgram_; }


private: /// ゲーム内オブジェクト

};