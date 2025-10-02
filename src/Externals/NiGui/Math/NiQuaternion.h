#pragma once

class NiVec3;

/// <summary>
/// NiQuaternion
/// </summary>
class NiQuaternion
{
public:
    /// データ
    float x,y,z,w;

    /// <returns>単位NiQuaternion</returns>
    static NiQuaternion Identity();

    /// <returns>共役NiQuaternion</returns>
    static NiQuaternion Conjugate(const NiQuaternion& _q);

    /// <returns>任意軸回転を表すNiQuaternionの生成</returns>
    static NiQuaternion RotateAxisAngleQuaternion(const NiVec3& _axis, float _angle);

    /// <returns>ノルム</returns>
    float Norm() const;

    /// <returns>正規化したNiQuaternion</returns>
    NiQuaternion Normalized() const;

    /// <returns>逆NiQuaternionを返す</returns>
    NiQuaternion Inversed() const;


    NiQuaternion operator +(const NiQuaternion& _rq) const;
    NiQuaternion operator -(const NiQuaternion& _rq) const;
    NiQuaternion operator *(const NiQuaternion& _rq) const;
    NiQuaternion operator /(float _f) const;

    NiQuaternion operator -() const;

    NiQuaternion operator *(float _f) const;

    /// <summary>
    /// 球面線形補間
    /// </summary>
    /// <param name="_begin">開始</param>
    /// <param name="_end">終了</param>
    /// <param name="_t">時間</param>
    static NiQuaternion Slerp(const NiQuaternion& _begin, const NiQuaternion& _end, float _t);
};

namespace FMath
{
    NiVec3 RotateVector(const NiVec3& _v, const NiQuaternion& _q);
}