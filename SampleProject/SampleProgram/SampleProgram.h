#pragma once

#include <Framework/NimaFramework.h>
#include <Timer/Timer.h>

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
    Timer globalTimer_;     // !< グローバルタイマー
};