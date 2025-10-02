#pragma once

class Vector3;

/// <summary>
/// Quaternion
/// </summary>
class Quaternion
{
public:
    /// データ
    float x,y,z,w;

    /// <returns>単位Quaternion</returns>
    static Quaternion Identity();

    /// <returns>共役Quaternion</returns>
    static Quaternion Conjugate(const Quaternion& _q);

    /// <returns>任意軸回転を表すQuaternionの生成</returns>
    static Quaternion RotateAxisAngleQuaternion(const Vector3& _axis, float _angle);

    /// <returns>ノルム</returns>
    float Norm() const;

    /// <returns>正規化したQuaternion</returns>
    Quaternion Normalized() const;

    /// <returns>逆Quaternionを返す</returns>
    Quaternion Inversed() const;


    Quaternion operator +(const Quaternion& _rq) const;
    Quaternion operator -(const Quaternion& _rq) const;
    Quaternion operator *(const Quaternion& _rq) const;
    Quaternion operator /(float _f) const;

    Quaternion operator -() const;

    Quaternion operator *(float _f) const;

    /// <summary>
    /// 球面線形補間
    /// </summary>
    /// <param name="_begin">開始</param>
    /// <param name="_end">終了</param>
    /// <param name="_t">時間</param>
    static Quaternion Slerp(const Quaternion& _begin, const Quaternion& _end, float _t);
};

namespace FMath
{
    Vector3 RotateVector(const Vector3& _v, const Quaternion& _q);
}