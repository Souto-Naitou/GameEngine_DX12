// Copyright © 2024 Souto-Naitou. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

#pragma once

#include "Vector2.h"

class Matrix4x4;

/// <summary>
/// 3th Dimension Vector
/// </summary>
class Vector3 final {
public:
    float x;
    float y;
    float z;

    Vector3() : x(), y(), z() {};

    Vector3(const float& _x, const float& _y, const float& _z)
    {
        x = _x;
        y = _y;
        z = _z;
        return;
    }

    Vector3(const float& _x, const float& _y)
    {
        x = _x;
        y = _y;
        z = 0.0f;
        return;
    }

    Vector3(const float& _x)
    {
        x = _x;
        y = 0.0f;
        z = 0.0f;
        return;
    }

    Vector3(const Vector2& _vec2, float _f)
    {
        x = _vec2.x;
        y = _vec2.y;
        z = _f;
        return;
    }

    Vector3(const Vector2& _vec2)
    {
        x = _vec2.x;
        y = _vec2.y;
        z = 0.0f;
        return;
    }

    Vector2 xy() const { return Vector2(x, y); }
    Vector2 xz() const { return Vector2(x, z); }
    Vector2 yz() const { return Vector2(y, z); }

    /// ===========
    /// calcuration
    /// ===========

    float   Dot(const Vector3& _v)                  const;
    Vector3 Cross(const Vector3& _v)                const;
    float   Length()                                const;
    float   LengthWithoutRoot()                     const;
    Vector3 Normalize()                             const;
    float   Distance(const Vector3& _destination)   const;
    float   Projection(const Vector3& _a)           const;
    void    Lerp(const Vector3& _begin, const Vector3& _end, float _t);
    void    Theta(float _azimuth, float _elevation, const Vector3& _origin = { 0.0f, 0.0f }) const;

    /// ==========
    /// minus sign
    /// ==========

    Vector3 operator-() const;

    /// =======
    /// Vector3
    /// =======

    Vector3& operator+=(const Vector3& _rv);
    Vector3& operator-=(const Vector3& _rv);
    Vector3& operator*=(const Vector3& _rv);

    Vector3 operator+(const Vector3& _v) const;
    Vector3 operator-(const Vector3& _v) const;
    Vector3 operator*(const Vector3& _v) const;

    /// =====
    /// float
    /// =====

    Vector3 operator+(float) = delete;
    Vector3 operator-(float) = delete;
    Vector3 operator*(float _f) const;
    Vector3 operator/(float _f) const;
    Vector3& operator+=(float) = delete;
    Vector3& operator-=(float) = delete;
    Vector3& operator*=(float _f);
    Vector3& operator/=(float _f);

    /// =======
    /// Vector2
    /// =======

    Vector3& operator+=(const Vector2& _rv);
    Vector3& operator-=(const Vector2& _rv);
    Vector3& operator*=(const Vector2& _rv);

    Vector3 operator+(const Vector2& _v) const;
    Vector3 operator-(const Vector2& _v) const;
    Vector3 operator*(const Vector2& _v) const;
};

Vector3 operator*(const float _f, const Vector3& _v);
Vector3 operator/(const float _f, const Vector3& _v);

namespace FMath
{
    Vector3 Add(const Vector3& _v1, const Vector3& _v2);
    Vector3 Subtract(const Vector3& _v1, const Vector3& _v2);
    Vector3 Multiply(float _scalar, const Vector3& _v);
    float   Dot(const Vector3& _v1, const Vector3& _v2);
    float   Length(const Vector3& _v);
    Vector3 Normalize(const Vector3& _v);
    Vector3 Transform(const Vector3& _vector, const Matrix4x4& _matrix);
}