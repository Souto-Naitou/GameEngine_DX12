// Copyright © 2024 Souto-Naitou. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

#pragma once

#include "NiVec2.h"

//class Matrix4x4;

/// <summary>
/// 3th Dimension Vector
/// </summary>
class NiVec3 final {
public:
    float x;
    float y;
    float z;

    NiVec3() : x(), y(), z() {};

    NiVec3(const float& _x, const float& _y, const float& _z)
    {
        x = _x;
        y = _y;
        z = _z;
        return;
    }

    NiVec3(const float& _x, const float& _y)
    {
        x = _x;
        y = _y;
        z = 0.0f;
        return;
    }

    NiVec3(const float& _x)
    {
        x = _x;
        y = 0.0f;
        z = 0.0f;
        return;
    }

    NiVec3(const NiVec2& _vec2, float _f)
    {
        x = _vec2.x;
        y = _vec2.y;
        z = _f;
        return;
    }

    NiVec3(const NiVec2& _vec2)
    {
        x = _vec2.x;
        y = _vec2.y;
        z = 0.0f;
        return;
    }

    NiVec2 xy() const { return NiVec2(x, y); }
    NiVec2 xz() const { return NiVec2(x, z); }
    NiVec2 yz() const { return NiVec2(y, z); }

    /// ===========
    /// calcuration
    /// ===========

    float   Dot(const NiVec3& _v)                  const;
    NiVec3  Cross(const NiVec3& _v)                const;
    float   Length()                                const;
    float   LengthWithoutRoot()                     const;
    NiVec3  Normalize()                             const;
    float   Distance(const NiVec3& _destination)   const;
    float   Projection(const NiVec3& _a)           const;
    void    Lerp(const NiVec3& _begin, const NiVec3& _end, float _t);
    void    Theta(float _azimuth, float _elevation, const NiVec3& _origin = { 0.0f, 0.0f }) const;

    /// ==========
    /// minus sign
    /// ==========

    NiVec3 operator-() const;

    /// =======
    /// NiVec3
    /// =======

    NiVec3& operator+=(const NiVec3& _rv);
    NiVec3& operator-=(const NiVec3& _rv);
    NiVec3& operator*=(const NiVec3& _rv);

    NiVec3 operator+(const NiVec3& _v) const;
    NiVec3 operator-(const NiVec3& _v) const;
    NiVec3 operator*(const NiVec3& _v) const;

    /// =====
    /// float
    /// =====

    NiVec3 operator+(float) = delete;
    NiVec3 operator-(float) = delete;
    NiVec3 operator*(float _f) const;
    NiVec3 operator/(float _f) const;
    NiVec3& operator+=(float) = delete;
    NiVec3& operator-=(float) = delete;
    NiVec3& operator*=(float _f);
    NiVec3& operator/=(float _f);

    /// =======
    /// NiVec2
    /// =======

    NiVec3& operator+=(const NiVec2& _rv);
    NiVec3& operator-=(const NiVec2& _rv);
    NiVec3& operator*=(const NiVec2& _rv);

    NiVec3 operator+(const NiVec2& _v) const;
    NiVec3 operator-(const NiVec2& _v) const;
    NiVec3 operator*(const NiVec2& _v) const;
};

NiVec3 operator*(const float _f, const NiVec3& _v);
NiVec3 operator/(const float _f, const NiVec3& _v);

namespace FMath
{
    NiVec3  Add(const NiVec3& _v1, const NiVec3& _v2);
    NiVec3  Subtract(const NiVec3& _v1, const NiVec3& _v2);
    NiVec3  Multiply(float _scalar, const NiVec3& _v);
    float   Dot(const NiVec3& _v1, const NiVec3& _v2);
    float   Length(const NiVec3& _v);
    NiVec3  Normalize(const NiVec3& _v);
    //NiVec3  Transform(const NiVec3& _vector, const Matrix4x4& _matrix);
}