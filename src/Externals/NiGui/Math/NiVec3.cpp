// Copyright © 2024 Souto-Naitou. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

#include "NiVec3.h"
//#include "../Matrix4x4.h"

#include <cassert>
#include <cmath>

/// ===========
/// calcuration
/// ===========

float NiVec3::Dot(const NiVec3& _v) const
{
    return float(
        x * _v.x +
        y * _v.y +
        z * _v.z
    );
}

NiVec3 NiVec3::Cross(const NiVec3& _v) const
{
    return NiVec3(
        y * _v.z - z * _v.y,
        z * _v.x - x * _v.z,
        x * _v.y - y * _v.x
    );
}

float NiVec3::Length() const
{
    return std::sqrtf(x * x + y * y + z * z);
}

float NiVec3::LengthWithoutRoot() const
{
    return x * x + y * y + z * z;
}

NiVec3 NiVec3::Normalize() const
{
    float length = this->Length();
    return NiVec3(
        x / length,
        y / length,
        z / length
    );
}

void NiVec3::Lerp(const NiVec3& _begin, const NiVec3& _end, float _t)
{
    x = (1.0f - _t) * _begin.x + _t * _end.x;
    y = (1.0f - _t) * _begin.y + _t * _end.y;
    z = (1.0f - _t) * _begin.z + _t * _end.z;
    return;
}

void NiVec3::Theta(float _azimuth, float _elevation, const NiVec3& _origin) const
{
    NiVec3 distance = *this - _origin;
    _azimuth = std::atan2(distance.y, distance.x);
    _elevation = std::atan2(distance.z, sqrtf(distance.x * distance.x + distance.y * distance.y));
    return;
}

float NiVec3::Projection(const NiVec3& _a) const
{
    NiVec3 axis = _a.Normalize();
    return (*this).Dot(axis);
}

float NiVec3::Distance(const NiVec3& _destination) const
{
    NiVec3 distance = _destination - *this;
    return distance.Length();
}

/// ==========
/// minus sign
/// ==========

NiVec3 NiVec3::operator-() const
{
    return NiVec3(-x, -y, -z);
}

/// =======
/// NiVec3
/// =======

NiVec3& NiVec3::operator+=(const NiVec3& _rv)
{
    x += _rv.x;
    y += _rv.y;
    z += _rv.z;
    return *this;
}

NiVec3& NiVec3::operator-=(const NiVec3& _rv)
{
    x -= _rv.x;
    y -= _rv.y;
    z -= _rv.z;
    return *this;
}

NiVec3& NiVec3::operator*=(const NiVec3& _rv)
{
    x *= _rv.x;
    y *= _rv.y;
    z *= _rv.z;
    return *this;
}

NiVec3 NiVec3::operator-(const NiVec3& _v) const
{
    NiVec3 result{};
    result.x = x - _v.x;
    result.y = y - _v.y;
    result.z = z - _v.z;
    return result;
}

NiVec3 NiVec3::operator*(const NiVec3& _v) const
{
    NiVec3 result{};
    result.x = x * _v.x;
    result.y = y * _v.y;
    result.z = z * _v.z;
    return result;
}

NiVec3 NiVec3::operator+(const NiVec3& _v) const
{
    NiVec3 result{};
    result.x = x + _v.x;
    result.y = y + _v.y;
    result.z = z + _v.z;
    return result;
}

/// =====
/// float
/// =====

NiVec3 NiVec3::operator*(float _f) const
{
    NiVec3 result{};
    result.x = x * _f;
    result.y = y * _f;
    result.z = z * _f;
    return result;
}

NiVec3 NiVec3::operator/(float _f) const
{
    NiVec3 result{};
    result.x = x / _f;
    result.y = y / _f;
    result.z = z / _f;
    return result;
}

NiVec3& NiVec3::operator*=(float _f)
{
    x *= _f;
    y *= _f;
    z *= _f;
    return *this;
}

NiVec3& NiVec3::operator/=(float _f)
{
    x /= _f;
    y /= _f;
    z /= _f;
    return *this;
}

/// =======
/// NiVec2
/// =======

NiVec3& NiVec3::operator+=(const NiVec2& _rv)
{
    x += _rv.x;
    y += _rv.y;
    return *this;
}

NiVec3& NiVec3::operator-=(const NiVec2& _rv)
{
    x -= _rv.x;
    y -= _rv.y;
    return *this;
}

NiVec3& NiVec3::operator*=(const NiVec2& _rv)
{
    x *= _rv.x;
    y *= _rv.y;
    return *this;
}

NiVec3 NiVec3::operator+(const NiVec2& _v) const
{
    NiVec3 result{};
    result.x = x + _v.x;
    result.y = y + _v.y;
    result.z = z;
    return result;
}

NiVec3 NiVec3::operator-(const NiVec2& _v) const
{
    NiVec3 result{};
    result.x = x - _v.x;
    result.y = y - _v.y;
    result.z = z;
    return result;
}

NiVec3 NiVec3::operator*(const NiVec2& _v) const
{
    NiVec3 result{};
    result.x = x * _v.x;
    result.y = y * _v.y;
    result.z = z;
    return result;
}

NiVec3 operator*(const float _f, const NiVec3& _v)
{
    return NiVec3(_v.x * _f, _v.y * _f, _v.z * _f);
}

NiVec3 operator/(const float _f, const NiVec3& _v)
{
    return NiVec3(_v.x / _f, _v.y / _f, _v.z / _f);
}