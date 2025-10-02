// Copyright © 2024 Souto-Naitou. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

#pragma once

#include "NiVec3.h"
#include "NiVec2.h"

/// <summary>
/// 4th Dimension Vector
/// </summary>
class alignas(16) NiVec4 final {
public:
    float x;
    float y;
    float z;
    float w;

    NiVec4() : x(), y(), z(), w() {};

    NiVec4(const float& _x, const float& _y, const float& _z, const float& _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
        return;
    }

    NiVec4(const float& _x, const float& _y, const float& _z)
    {
        x = _x;
        y = _y;
        z = _z;
        w = 0.0f;
        return;
    }

    NiVec4(const float& _x, const float& _y)
    {
        x = _x;
        y = _y;
        z = 0.0f;
        w = 0.0f;
        return;
    }

    NiVec4(const float& _x)
    {
        x = _x;
        y = 0.0f;
        z = 0.0f;
        w = 0.0f;
        return;
    }

    NiVec4(const NiVec3& _vec3)
    {
        x = _vec3.x;
        y = _vec3.y;
        z = _vec3.z;
        w = 0.0f;
        return;
    }

    NiVec4(const NiVec2& _vec2)
    {
        x = _vec2.x;
        y = _vec2.y;
        z = 0.0f;
        w = 0.0f;
        return;
    }

    NiVec4(const NiVec3& _vec3, const float& _w)
    {
        x = _vec3.x;
        y = _vec3.y;
        z = _vec3.z;
        w = _w;
        return;
    }

    NiVec3 xyz() const { return NiVec3(x, y, z); }

    NiVec2 xy() const { return NiVec2(x, y); }


    void    Lerp(const NiVec4& _begin, const NiVec4& _end, float _t);


    /// マイナス符号
    NiVec4 operator-() const;

    /// =======
    /// NiVec4
    /// =======

    NiVec4& operator+=(const NiVec4& _rv);
    NiVec4& operator-=(const NiVec4& _rv);
    NiVec4& operator*=(const NiVec4& _rv);

    NiVec4 operator+(const NiVec4& _v);
    NiVec4 operator-(const NiVec4& _v);
    NiVec4 operator*(const NiVec4& _v);

    /// =====
    /// float
    /// =====

    NiVec4 operator+(float) = delete;
    NiVec4 operator-(float) = delete;
    NiVec4 operator*(float _f);
    NiVec4 operator/(float _f);
    NiVec4& operator+=(float) = delete;
    NiVec4& operator-=(float) = delete;
    NiVec4& operator*=(float _f);
    NiVec4& operator/=(float _f);

    /// =======
    /// NiVec2
    /// =======

    NiVec4& operator+=(const NiVec2& _rv);
    NiVec4& operator-=(const NiVec2& _rv);
    NiVec4& operator*=(const NiVec2& _rv);

    NiVec4 operator+(const NiVec2& _v);
    NiVec4 operator-(const NiVec2& _v);
    NiVec4 operator*(const NiVec2& _v);

    /// =======
    /// NiVec3
    /// =======

    NiVec4& operator+=(const NiVec3& _rv);
    NiVec4& operator-=(const NiVec3& _rv);
    NiVec4& operator*=(const NiVec3& _rv);

    NiVec4 operator+(const NiVec3& _v);
    NiVec4 operator-(const NiVec3& _v);
    NiVec4 operator*(const NiVec3& _v);

};