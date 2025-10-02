#pragma once

#include "Vector3.h"

enum class Shape
{
    Sphere,
    Polygon,
    OBB,
    AABB,
};

struct Sphere
{
    Vector3 center; // 中心点
    float radius; // 半径
};