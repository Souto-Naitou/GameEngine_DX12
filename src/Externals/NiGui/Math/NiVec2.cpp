// Copyright © 2024 Souto-Naitou. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

#include "NiVec2.h"

#include <cmath>

/// ===========
/// calcuration
/// ===========

float NiVec2::Dot(const NiVec2& _v) const
{
    return x * _v.x + y * _v.y;
}

float NiVec2::Cross(const NiVec2& _v) const
{
    return x * _v.y - y * _v.x;
}

float NiVec2::Length() const
{
    return std::sqrtf(x * x + y * y);
}

float NiVec2::LengthWithoutRoot() const
{
    return x * x + y * y;
}

NiVec2 NiVec2::Perpendicular() const
{
    return NiVec2(-y, x);
}

NiVec2 NiVec2::Normalize() const
{
    float length = this->Length();
    if (length == 0) return {};
    return NiVec2(
        x / length,
        y / length
    );
}

float NiVec2::Projection(const NiVec2& _a) const
{
    NiVec2 axis = _a.Normalize();
    return (*this).Dot(axis);
}

void NiVec2::Lerp(const NiVec2& _begin, const NiVec2& _end, float _t)
{
    x = (1.0f - _t) * _begin.x + _t * _end.x;
    y = (1.0f - _t) * _begin.y + _t * _end.y;
    return;
}

float NiVec2::Distance(const NiVec2& _destination) const
{
    return NiVec2(_destination - *this).Length();
}

float NiVec2::Theta(const NiVec2& _origin) const
{
    NiVec2 distance = *this - _origin;
    return std::atan2(distance.y, distance.x);
}

NiVec2 NiVec2::Rotated(float _theta) const
{
    NiVec2 result = {};
    result.x = x * std::cosf(_theta) - y * std::sinf(_theta);
    result.y = x * std::sinf(_theta) + y * std::cosf(_theta);
    return result;
}

NiVec2 NiVec2::operator-() const
{
    return NiVec2(-x, -y);
}

/// =====
/// float
/// =====

NiVec2 NiVec2::operator*(float _f) const
{
    NiVec2 result{};
    result.x = x * _f;
    result.y = y * _f;
    return result;
}

NiVec2 NiVec2::operator/(float _f) const
{
    NiVec2 result{};
    result.x = x / _f;
    result.y = y / _f;
    return result;
}

NiVec2& NiVec2::operator*=(float _f)
{
    x *= _f;
    y *= _f;
    return *this;
}

NiVec2& NiVec2::operator/=(float _f)
{
    x /= _f;
    y /= _f;
    return *this;
}

/// =======
/// NiVec2
/// =======

NiVec2 NiVec2::operator+(const NiVec2& _v) const
{
    NiVec2 result{};
    result.x = x + _v.x;
    result.y = y + _v.y;
    return result;
}

NiVec2 NiVec2::operator-(const NiVec2& _v) const
{
    NiVec2 result{};
    result.x = x - _v.x;
    result.y = y - _v.y;
    return result;
}

NiVec2 NiVec2::operator*(const NiVec2& _v) const
{
    NiVec2 result{};
    result.x = x * _v.x;
    result.y = y * _v.y;
    return result;
}

NiVec2& NiVec2::operator+=(const NiVec2& _v)
{
    x += _v.x;
    y += _v.y;
    return *this;
}

NiVec2& NiVec2::operator-=(const NiVec2& _v)
{
    x -= _v.x;
    y -= _v.y;
    return *this;
}

NiVec2& NiVec2::operator*=(const NiVec2& _v)
{
    x *= _v.x;
    y *= _v.y;
    return *this;
}

bool NiVec2::operator==(const NiVec2& _v) const
{
    return x == _v.x && y == _v.y;
}

/// =========
/// overscope
/// =========

NiVec2 operator*(const float _f, const NiVec2& _v)
{
    return NiVec2(_v.x * _f, _v.y * _f);
}

NiVec2 operator/(const float _f, const NiVec2& _v)
{
    return NiVec2(_v.x / _f, _v.y / _f);
}
