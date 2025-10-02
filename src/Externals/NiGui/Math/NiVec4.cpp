// Copyright © 2024 Souto-Naitou. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

#include "NiVec4.h"

NiVec4 NiVec4::operator-() const
{
    return NiVec4(-x, -y, -z, -w);
}

void NiVec4::Lerp(const NiVec4& _begin, const NiVec4& _end, float _t)
{
    x = _begin.x + (_end.x - _begin.x) * _t;
    y = _begin.y + (_end.y - _begin.y) * _t;
    z = _begin.z + (_end.z - _begin.z) * _t;
    w = _begin.w + (_end.w - _begin.w) * _t;
    return;
}

/// =======
/// NiVec4
/// =======

NiVec4& NiVec4::operator+=(const NiVec4& _rv)
{
    x += _rv.x;
    y += _rv.y;
    z += _rv.z;
    w += _rv.w;
    return *this;
}

NiVec4& NiVec4::operator-=(const NiVec4& _rv)
{
    x -= _rv.x;
    y -= _rv.y;
    z -= _rv.z;
    w -= _rv.w;
    return *this;
}

NiVec4& NiVec4::operator*=(const NiVec4& _rv)
{
    x *= _rv.x;
    y *= _rv.y;
    z *= _rv.z;
    w *= _rv.w;
    return *this;
}

NiVec4 NiVec4::operator+(const NiVec4& _v)
{
    NiVec4 result{};
    result.x = x + _v.x;
    result.y = y + _v.y;
    result.z = z + _v.z;
    result.w = w + _v.w;
    return result;
}


NiVec4 NiVec4::operator-(const NiVec4& _v)
{
    NiVec4 result{};
    result.x = x - _v.x;
    result.y = y - _v.y;
    result.z = z - _v.z;
    result.w = w - _v.w;
    return result;
}

NiVec4 NiVec4::operator*(const NiVec4& _v)
{
    NiVec4 result{};
    result.x = x * _v.x;
    result.y = y * _v.y;
    result.z = z * _v.z;
    result.w = w * _v.w;
    return result;
}

/// =====
/// float
/// =====


NiVec4 NiVec4::operator*(float _f)
{
    NiVec4 result{};
    result.x = x * _f;
    result.y = y * _f;
    result.z = z * _f;
    result.w = w * _f;
    return result;
}

NiVec4 NiVec4::operator/(float _f)
{
    NiVec4 result{};
    result.x = x / _f;
    result.y = y / _f;
    result.z = z / _f;
    result.w = w / _f;
    return result;
}

NiVec4& NiVec4::operator*=(float _f)
{
    x *= _f;
    y *= _f;
    z *= _f;
    w += _f;
    return *this;
}

NiVec4& NiVec4::operator/=(float _f)
{
    x /= _f;
    y /= _f;
    z /= _f;
    w /= _f;
    return *this;
}

/// =======
/// NiVec2
/// =======

NiVec4& NiVec4::operator+=(const NiVec2& _rv)
{
    x += _rv.x;
    y += _rv.y;
    return *this;
}

NiVec4& NiVec4::operator-=(const NiVec2& _rv)
{
    x -= _rv.x;
    y -= _rv.y;
    return *this;
}

NiVec4& NiVec4::operator*=(const NiVec2& _rv)
{
    x *= _rv.x;
    y *= _rv.y;
    return *this;
}

NiVec4 NiVec4::operator+(const NiVec2& _v)
{
    NiVec4 result{};
    result.x = x + _v.x;
    result.y = y + _v.y;
    result.z = z;
    result.w = w;
    return result;
}

NiVec4 NiVec4::operator-(const NiVec2& _v)
{
    NiVec4 result{};
    result.x = x - _v.x;
    result.y = y - _v.y;
    result.z = z;
    result.w = w;
    return result;
}

NiVec4 NiVec4::operator*(const NiVec2& _v)
{
    NiVec4 result{};
    result.x = x * _v.x;
    result.y = y * _v.y;
    result.z = z;
    result.w = w;
    return result;
}

/// =======
/// NiVec3
/// =======

NiVec4& NiVec4::operator+=(const NiVec3& _rv)
{
    x += _rv.x;
    y += _rv.y;
    z += _rv.z;
    return *this;
}

NiVec4& NiVec4::operator-=(const NiVec3& _rv)
{
    x -= _rv.x;
    y -= _rv.y;
    z -= _rv.z;
    return *this;
}

NiVec4& NiVec4::operator*=(const NiVec3& _rv)
{
    x *= _rv.x;
    y *= _rv.y;
    z *= _rv.z;
    return *this;
}

NiVec4 NiVec4::operator+(const NiVec3& _v)
{
    NiVec4 result{};
    result.x = x + _v.x;
    result.y = y + _v.y;
    result.z = z + _v.z;
    result.w = w;
    return result;
}

NiVec4 NiVec4::operator-(const NiVec3& _v)
{
    NiVec4 result{};
    result.x = x - _v.x;
    result.y = y - _v.y;
    result.z = z - _v.z;
    result.w = w;
    return result;
}

NiVec4 NiVec4::operator*(const NiVec3& _v)
{
    NiVec4 result{};
    result.x = x * _v.x;
    result.y = y * _v.y;
    result.z = z * _v.z;
    result.w = w;
    return result;
}

/// =========
/// overscope
/// =========

NiVec4 operator*(const float _f, const NiVec4& _v)
{
    return NiVec4(_v.x * _f, _v.y * _f, _v.z * _f, _v.w * _f);
}

NiVec4 operator/(const float _f, const NiVec4& _v)
{
    return NiVec4(_v.x / _f, _v.y / _f, _v.z / _f, _v.w / _f);
}
