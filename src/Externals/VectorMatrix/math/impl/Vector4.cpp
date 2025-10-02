// Copyright © 2024 Souto-Naitou. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

#include "../Vector4.h"

Vector4 Vector4::operator-() const
{
    return Vector4(-x, -y, -z, -w);
}

void Vector4::Lerp(const Vector4& _begin, const Vector4& _end, float _t)
{
    x = _begin.x + (_end.x - _begin.x) * _t;
    y = _begin.y + (_end.y - _begin.y) * _t;
    z = _begin.z + (_end.z - _begin.z) * _t;
    w = _begin.w + (_end.w - _begin.w) * _t;
    return;
}

/// =======
/// Vector4
/// =======

Vector4& Vector4::operator+=(const Vector4& _rv)
{
    x += _rv.x;
    y += _rv.y;
    z += _rv.z;
    w += _rv.w;
    return *this;
}

Vector4& Vector4::operator-=(const Vector4& _rv)
{
    x -= _rv.x;
    y -= _rv.y;
    z -= _rv.z;
    w -= _rv.w;
    return *this;
}

Vector4& Vector4::operator*=(const Vector4& _rv)
{
    x *= _rv.x;
    y *= _rv.y;
    z *= _rv.z;
    w *= _rv.w;
    return *this;
}

Vector4 Vector4::operator+(const Vector4& _v)
{
    Vector4 result{};
    result.x = x + _v.x;
    result.y = y + _v.y;
    result.z = z + _v.z;
    result.w = w + _v.w;
    return result;
}


Vector4 Vector4::operator-(const Vector4& _v)
{
    Vector4 result{};
    result.x = x - _v.x;
    result.y = y - _v.y;
    result.z = z - _v.z;
    result.w = w - _v.w;
    return result;
}

Vector4 Vector4::operator*(const Vector4& _v)
{
    Vector4 result{};
    result.x = x * _v.x;
    result.y = y * _v.y;
    result.z = z * _v.z;
    result.w = w * _v.w;
    return result;
}

/// =====
/// float
/// =====


Vector4 Vector4::operator*(float _f)
{
    Vector4 result{};
    result.x = x * _f;
    result.y = y * _f;
    result.z = z * _f;
    result.w = w * _f;
    return result;
}

Vector4 Vector4::operator/(float _f)
{
    Vector4 result{};
    result.x = x / _f;
    result.y = y / _f;
    result.z = z / _f;
    result.w = w / _f;
    return result;
}

Vector4& Vector4::operator*=(float _f)
{
    x *= _f;
    y *= _f;
    z *= _f;
    w += _f;
    return *this;
}

Vector4& Vector4::operator/=(float _f)
{
    x /= _f;
    y /= _f;
    z /= _f;
    w /= _f;
    return *this;
}

/// =======
/// Vector2
/// =======

Vector4& Vector4::operator+=(const Vector2& _rv)
{
    x += _rv.x;
    y += _rv.y;
    return *this;
}

Vector4& Vector4::operator-=(const Vector2& _rv)
{
    x -= _rv.x;
    y -= _rv.y;
    return *this;
}

Vector4& Vector4::operator*=(const Vector2& _rv)
{
    x *= _rv.x;
    y *= _rv.y;
    return *this;
}

Vector4 Vector4::operator+(const Vector2& _v)
{
    Vector4 result{};
    result.x = x + _v.x;
    result.y = y + _v.y;
    result.z = z;
    result.w = w;
    return result;
}

Vector4 Vector4::operator-(const Vector2& _v)
{
    Vector4 result{};
    result.x = x - _v.x;
    result.y = y - _v.y;
    result.z = z;
    result.w = w;
    return result;
}

Vector4 Vector4::operator*(const Vector2& _v)
{
    Vector4 result{};
    result.x = x * _v.x;
    result.y = y * _v.y;
    result.z = z;
    result.w = w;
    return result;
}

/// =======
/// Vector3
/// =======

Vector4& Vector4::operator+=(const Vector3& _rv)
{
    x += _rv.x;
    y += _rv.y;
    z += _rv.z;
    return *this;
}

Vector4& Vector4::operator-=(const Vector3& _rv)
{
    x -= _rv.x;
    y -= _rv.y;
    z -= _rv.z;
    return *this;
}

Vector4& Vector4::operator*=(const Vector3& _rv)
{
    x *= _rv.x;
    y *= _rv.y;
    z *= _rv.z;
    return *this;
}

Vector4 Vector4::operator+(const Vector3& _v)
{
    Vector4 result{};
    result.x = x + _v.x;
    result.y = y + _v.y;
    result.z = z + _v.z;
    result.w = w;
    return result;
}

Vector4 Vector4::operator-(const Vector3& _v)
{
    Vector4 result{};
    result.x = x - _v.x;
    result.y = y - _v.y;
    result.z = z - _v.z;
    result.w = w;
    return result;
}

Vector4 Vector4::operator*(const Vector3& _v)
{
    Vector4 result{};
    result.x = x * _v.x;
    result.y = y * _v.y;
    result.z = z * _v.z;
    result.w = w;
    return result;
}

/// =========
/// overscope
/// =========

Vector4 operator*(const float _f, const Vector4& _v)
{
    return Vector4(_v.x * _f, _v.y * _f, _v.z * _f, _v.w * _f);
}

Vector4 operator/(const float _f, const Vector4& _v)
{
    return Vector4(_v.x / _f, _v.y / _f, _v.z / _f, _v.w / _f);
}
