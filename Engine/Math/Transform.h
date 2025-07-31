#pragma once
#include <Vector3.h>
#include <Quaternion.h>
#include <nlohmann/json.hpp>

struct EulerTransform
{
    Vector3 scale;
    Vector3 rotate;
    Vector3 translate;
};

struct QuaternionTransform
{
    Vector3 scale;
    Quaternion rotate;
    Vector3 translate;
};