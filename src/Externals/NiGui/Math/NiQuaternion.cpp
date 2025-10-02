#include "./NiQuaternion.h"

#include "./NiVec3.h"

#include <cmath> // std::sqrt

NiQuaternion NiQuaternion::Identity()
{
    return NiQuaternion({ 0.0f, 0.0f, 0.0f, 1.0f });
}


NiQuaternion NiQuaternion::Conjugate(const NiQuaternion& _q)
{
    return NiQuaternion({ -_q.x, -_q.y, -_q.z, _q.w });
}

float NiQuaternion::Norm() const
{
    return std::sqrt(x * x + y * y + z * z + w * w);
}

NiQuaternion NiQuaternion::Normalized() const
{
    return *this / Norm();
}


NiQuaternion NiQuaternion::Inversed() const
{
    return Conjugate(*this) / (x * x + y * y + z * z + w * w);
}

NiQuaternion NiQuaternion::RotateAxisAngleQuaternion(const NiVec3& _axis, float _angle)
{
    float halfAngle = _angle * 0.5f;
    float cosHalfAngle = std::cos(halfAngle);
    float sinHalfAngle = std::sin(halfAngle);

    NiQuaternion result = {};
    result.x = sinHalfAngle * _axis.x;
    result.y = sinHalfAngle * _axis.y;
    result.z = sinHalfAngle * _axis.z;
    result.w = cosHalfAngle;

    return result;
}

NiQuaternion NiQuaternion::operator+(const NiQuaternion& _rq) const
{
    return NiQuaternion({ x + _rq.x, y + _rq.y, z + _rq.z, w + _rq.w });
}

NiQuaternion NiQuaternion::operator-(const NiQuaternion& _rq) const
{
    return NiQuaternion({ x - _rq.x, y - _rq.y, z - _rq.z, w - _rq.w });
}

NiQuaternion NiQuaternion::operator*(const NiQuaternion& _rq) const
{
    NiQuaternion result = {};
    result.x = w * _rq.x + x * _rq.w + y * _rq.z - z * _rq.y;
    result.y = w * _rq.y + y * _rq.w + z * _rq.x - x * _rq.z;
    result.z = w * _rq.z + z * _rq.w + x * _rq.y - y * _rq.x;
    result.w = w * _rq.w - x * _rq.x - y * _rq.y - z * _rq.z;

    return result;
}

NiQuaternion NiQuaternion::operator/(float _f) const
{
    NiQuaternion result = {};
    result.x = x / _f;
    result.y = y / _f;
    result.z = z / _f;
    result.w = w / _f;

    return result;
}

NiQuaternion NiQuaternion::operator-() const
{
    return NiQuaternion({ -x, -y, -z, -w });
}

NiQuaternion NiQuaternion::operator*(float _f) const
{
    return NiQuaternion({ x * _f, y * _f, z * _f, w * _f });
}

NiVec3 FMath::RotateVector(const NiVec3& _v, const NiQuaternion& _q)
{
    NiVec3 result = {};

    NiQuaternion q = _q * NiQuaternion({ _v.x, _v.y, _v.z, 0.0f }) * NiQuaternion::Conjugate(_q);

    result.x = q.x;
    result.y = q.y;
    result.z = q.z;

    return result;
}

NiQuaternion NiQuaternion::Slerp(const NiQuaternion& _begin, const NiQuaternion& _end, float _t)
{
    float dot = _begin.x * _end.x + _begin.y * _end.y + _begin.z * _end.z + _begin.w * _end.w;

    NiQuaternion begin = _begin;

    if (dot < 0.0f)
    {
        begin = -_begin;
        dot = -dot;
    }

    /// なす角を求める
    float theta = std::acos(dot);

    /// thetaとsinを使って補間係数scale0, scale1を求める
    float sinTheta = std::sin(theta);
    float scale0 = std::sin((1.0f - _t) * theta) / sinTheta;
    float scale1 = std::sin(_t * theta) / sinTheta;

    return begin * scale0 + _end * scale1;
}
