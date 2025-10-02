// Copyright © 2024 Souto-Naitou. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

#include "../Vector3.h"
#include "../Matrix4x4.h"

#include <assert.h>
#include <math.h>

Vector3 FMath::Add(const Vector3& _v1, const Vector3& _v2)
{
    return Vector3(
        _v1.x + _v2.x,
        _v1.y + _v2.y,
        _v1.z + _v2.z
    );
}

Vector3 FMath::Subtract(const Vector3& _v1, const Vector3& _v2)
{
    return Vector3(
        _v1.x - _v2.x,
        _v1.y - _v2.y,
        _v1.z - _v2.z
    );
}

Vector3 FMath::Multiply(float _scalar, const Vector3& _v)
{
    return Vector3(
        _scalar * _v.x,
        _scalar * _v.y,
        _scalar * _v.z
    );
}

float FMath::Dot(const Vector3& _v1, const Vector3& _v2)
{
    return float(
        _v1.x * _v2.x +
        _v1.y * _v2.y +
        _v1.z * _v2.z
        );
}

float FMath::Length(const Vector3& _v)
{
    return sqrtf(
        _v.x * _v.x +
        _v.y * _v.y +
        _v.z * _v.z
        );
}

Vector3 FMath::Normalize(const Vector3& _v)
{
    float length = Length(_v);
    return Vector3(
        _v.x / length,
        _v.y / length,
        _v.z / length
        );
}

Vector3 FMath::Transform(const Vector3& _vector, const Matrix4x4& _matrix)
{
    Vector3 result{};
    result.x =
        _vector.x * _matrix.m[0][0] +
        _vector.y * _matrix.m[1][0] +
        _vector.z * _matrix.m[2][0] +
        1.0f * _matrix.m[3][0];
    result.y =
        _vector.x * _matrix.m[0][1] +
        _vector.y * _matrix.m[1][1] +
        _vector.z * _matrix.m[2][1] +
        1.0f * _matrix.m[3][1];
    result.z =
        _vector.x * _matrix.m[0][2] +
        _vector.y * _matrix.m[1][2] +
        _vector.z * _matrix.m[2][2] +
        1.0f * _matrix.m[3][2];
    float w =
        _vector.x * _matrix.m[0][3] +
        _vector.y * _matrix.m[1][3] +
        _vector.z * _matrix.m[2][3] +
        1.0f * _matrix.m[3][3];
    assert(w != 0.0f);
    result.x /= w;
    result.y /= w;
    result.z /= w;
    return result;
}
