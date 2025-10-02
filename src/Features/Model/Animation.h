#pragma once
#include <Vector3.h>
#include <Quaternion.h>
#include <vector>
#include <unordered_map>
#include <string>

template <typename _Ty>
struct Keyframe
{
    float time = 0.0f;
    _Ty value = {};
};

template <typename _Ty>
struct AnimationCurve
{
    std::vector<Keyframe<_Ty>> keyframes;
};

struct NodeAnimation
{
    AnimationCurve<Vector3> scale = {};
    AnimationCurve<Quaternion> rotation = {};
    AnimationCurve<Vector3> translate = {};
};

struct Animation
{
    float duration = 0.0f; // アニメーションの総時間
    std::unordered_map<std::string, NodeAnimation> nodeAnimations; // ノードごとのアニメーションデータ
};
