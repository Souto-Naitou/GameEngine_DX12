#pragma once
#include <functional>
#include <imgui.h>
#include <string>

// アニメーションの補間を管理するクラス
//   ValueTypeはUpdate関数内で+-*を使用するため、演算子オーバーロードが必要

template <typename ValueType>
class AnimationTween
{
public:
    AnimationTween(float startSec, float durationSec, const ValueType& startValue, const ValueType& targetValue) :
        startSec_(startSec),
        durationSec_(durationSec),
        startValue_(startValue),
        targetValue_(targetValue)
    {};

    ~AnimationTween() = default;

    // 補間関数を設定
    inline void SetTransitionFunction(const std::function<float(float)>& func)
    {
        transitionFunction_ = func;
    }

    // アニメーションの開始時間を取得
    inline float GetStartSec() const { return startSec_; }

    // アニメーションが終了したかどうかを判定
    bool IsFinished(float currentTime) const
    {
        return currentTime >= (startSec_ + durationSec_);
    }

    // アニメーションの更新
    void Update(float currentTime, ValueType& currentValue);

    // ImGuiでの表示
    void ImGui(const std::string& name);

private:
    float           startSec_       = 0.0f;
    float           durationSec_    = 0.0f;
    const ValueType targetValue_    = {};
    const ValueType startValue_     = {};

    // 補間関数
    std::function<float(float)> transitionFunction_ = nullptr;
};

template<typename ValueType>
inline void AnimationTween<ValueType>::Update(float currentTime, ValueType& currentValue)
{
    if (currentTime < startSec_) return;

    float t = (currentTime - startSec_) / durationSec_;
    if (t > 1.0f) t = 1.0f;

    // 補間関数が設定されていれば適用
    if (transitionFunction_)
    {
        t = transitionFunction_(t);
    }

    currentValue = startValue_ + (targetValue_ - startValue_) * t; // ここでは単純な線形補間を仮定
}

template<typename ValueType>
inline void AnimationTween<ValueType>::ImGui(const std::string& name)
{
    #ifdef _DEBUG

    if (ImGui::TreeNode(name.c_str()))
    {
        ImGui::Indent(15.0f);

        ImGui::DragFloat("Start Sec", &startSec_, 0.01f, 0.0f, 100.0f, "%.2f");
        ImGui::DragFloat("Duration Sec", &durationSec_, 0.01f, 0.0f, 100.0f, "%.2f");

        ImGui::Unindent(15.0f);
        ImGui::TreePop();
    }

    #endif // _DEBUG
}