#include "Vector3.h"
// Copyright © 2024 Souto-Naitou. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

#include "../Vector3.h"
#include "../Matrix4x4.h"

#include <cassert>
#include <cmath>

/// ===========
/// calcuration
/// ===========

float Vector3::Dot(const Vector3& _v) const
{
    return float(
        x * _v.x +
        y * _v.y +
        z * _v.z
    );
}

Vector3 Vector3::Cross(const Vector3& _v) const
{
    return Vector3(
        y * _v.z - z * _v.y,
        z * _v.x - x * _v.z,
        x * _v.y - y * _v.x
    );
}

float Vector3::Length() const
{
    return std::sqrtf(x * x + y * y + z * z);
}

float Vector3::LengthWithoutRoot() const
{
    return x * x + y * y + z * z;
}

Vector3 Vector3::Normalize() const
{
    float length = this->Length();
    if (length == 0.0f) return {};
    return Vector3(
        x / length,
        y / length,
        z / length
    );
}

void Vector3::Lerp(const Vector3& _begin, const Vector3& _end, float _t)
{
    x = (1.0f - _t) * _begin.x + _t * _end.x;
    y = (1.0f - _t) * _begin.y + _t * _end.y;
    z = (1.0f - _t) * _begin.z + _t * _end.z;
    return;
}

void Vector3::Theta(float _azimuth, float _elevation, const Vector3& _origin) const
{
    Vector3 distance = *this - _origin;
    _azimuth = std::atan2(distance.y, distance.x);
    _elevation = std::atan2(distance.z, sqrtf(distance.x * distance.x + distance.y * distance.y));
    return;
}

float Vector3::Projection(const Vector3& _a) const
{
    Vector3 axis = _a.Normalize();
    return (*this).Dot(axis);
}

float Vector3::Distance(const Vector3& _destination) const
{
    Vector3 distance = _destination - *this;
    return distance.Length();
}

/// ==========
/// minus sign
/// ==========

Vector3 Vector3::operator-() const
{
    return Vector3(-x, -y, -z);
}

/// =======
/// Vector3
/// =======

Vector3& Vector3::operator+=(const Vector3& _rv)
{
    x += _rv.x;
    y += _rv.y;
    z += _rv.z;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& _rv)
{
    x -= _rv.x;
    y -= _rv.y;
    z -= _rv.z;
    return *this;
}

Vector3& Vector3::operator*=(const Vector3& _rv)
{
    x *= _rv.x;
    y *= _rv.y;
    z *= _rv.z;
    return *this;
}

Vector3 Vector3::operator-(const Vector3& _v) const
{
    Vector3 result{};
    result.x = x - _v.x;
    result.y = y - _v.y;
    result.z = z - _v.z;
    return result;
}

Vector3 Vector3::operator*(const Vector3& _v) const
{
    Vector3 result{};
    result.x = x * _v.x;
    result.y = y * _v.y;
    result.z = z * _v.z;
    return result;
}

Vector3 Vector3::operator+(const Vector3& _v) const
{
    Vector3 result{};
    result.x = x + _v.x;
    result.y = y + _v.y;
    result.z = z + _v.z;
    return result;
}

/// =====
/// float
/// =====

Vector3 Vector3::operator*(float _f) const
{
    Vector3 result{};
    result.x = x * _f;
    result.y = y * _f;
    result.z = z * _f;
    return result;
}

Vector3 Vector3::operator/(float _f) const
{
    Vector3 result{};
    result.x = x / _f;
    result.y = y / _f;
    result.z = z / _f;
    return result;
}

Vector3& Vector3::operator*=(float _f)
{
    x *= _f;
    y *= _f;
    z *= _f;
    return *this;
}

Vector3& Vector3::operator/=(float _f)
{
    x /= _f;
    y /= _f;
    z /= _f;
    return *this;
}

/// =======
/// Vector2
/// =======

Vector3& Vector3::operator+=(const Vector2& _rv)
{
    x += _rv.x;
    y += _rv.y;
    return *this;
}

Vector3& Vector3::operator-=(const Vector2& _rv)
{
    x -= _rv.x;
    y -= _rv.y;
    return *this;
}

Vector3& Vector3::operator*=(const Vector2& _rv)
{
    x *= _rv.x;
    y *= _rv.y;
    return *this;
}

Vector3 Vector3::operator+(const Vector2& _v) const
{
    Vector3 result{};
    result.x = x + _v.x;
    result.y = y + _v.y;
    result.z = z;
    return result;
}

Vector3 Vector3::operator-(const Vector2& _v) const
{
    Vector3 result{};
    result.x = x - _v.x;
    result.y = y - _v.y;
    result.z = z;
    return result;
}

Vector3 Vector3::operator*(const Vector2& _v) const
{
    Vector3 result{};
    result.x = x * _v.x;
    result.y = y * _v.y;
    result.z = z;
    return result;
}

Vector3 operator*(const float _f, const Vector3& _v)
{
    return Vector3(_v.x * _f, _v.y * _f, _v.z * _f);
}

Vector3 operator/(const float _f, const Vector3& _v)
{
    return Vector3(_v.x / _f, _v.y / _f, _v.z / _f);
}