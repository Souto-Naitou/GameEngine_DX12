#pragma once

#include <vector>

#include <Features/Model/Animation.h>
#include <stdexcept>
#include <Vector3.h>
#include <Quaternion.h>

namespace Helper::Animation
{
    Vector3 CalculateValue(const AnimationCurve<Vector3>& _keyframes, float _time);
    Quaternion CalculateValue(const AnimationCurve<Quaternion>& _keyframes, float _time);
}

