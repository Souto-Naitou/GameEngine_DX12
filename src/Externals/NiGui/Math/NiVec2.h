// Copyright © 2024 Souto-Naitou. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

#pragma once

/// <summary>
/// 2th Dimension Vector
/// </summary>
class NiVec2 final{
public:
    float x;
    float y;

    NiVec2() : x(), y() {};

    NiVec2(const unsigned int _x, const unsigned int& _y)
    {
        x = static_cast<float>(_x);
        y = static_cast<float>(_y);
        return;
    }

    NiVec2(const int& _x, const int& _y)
    {
        x = static_cast<float>(_x);
        y = static_cast<float>(_y);
        return;
    }

    NiVec2(const float& _x, const float& _y)
    {
        x = _x;
        y = _y;
        return;
    }

    NiVec2(const float& _x)
    {
        x = _x;
        y = 0.0f;
        return;
    }

    /// ===========
    /// calcuration
    /// ===========

    /// <summary>
    /// 内積
    /// </summary>
    /// <param name="_v">ベクトル</param>
    /// <returns>内積</returns>
    float       Dot(const NiVec2& _v)                          const;

    /// <summary>
    /// 外積
    /// </summary>
    /// <param name="_v">ベクトル</param>
    /// <returns>外積</returns>
    float       Cross(const NiVec2& _v)                        const;

    /// <summary>
    /// 原点からの距離を求める
    /// </summary>
    /// <returns>距離</returns>
    float       Length()                                        const;

    /// <summary>
    /// Length関数の平方根なし
    /// </summary>
    /// <returns></returns>
    float       LengthWithoutRoot()                             const;

    /// <summary>
    /// 垂直ベクトルを返す
    /// </summary>
    /// <returns></returns>
    NiVec2     Perpendicular()                                 const;

    /// <summary>
    /// 正規化済みの値を返す
    /// </summary>
    /// <returns></returns>
    NiVec2     Normalize()                                     const;

    /// <summary>
    /// thisから引数のベクトルまでの距離
    /// </summary>
    /// <param name="_destination">行き先</param>
    /// <returns>距離</returns>
    float       Distance(const NiVec2& _destination)           const;
    float       Theta(const NiVec2& _origin = { 0.0f, 0.0f })  const;
    NiVec2     Rotated(float _theta)                           const;

    /// <summary>
    /// 軸aに投影した値を返す
    /// </summary>
    /// <param name="_a">軸</param>
    /// <returns></returns>
    float       Projection(const NiVec2& _a)                   const;

    /// <summary>
    /// 線形補間
    /// </summary>
    /// <param name="_begin">初期値</param>
    /// <param name="_end">終了値</param>
    /// <param name="_t">0.0~1.0</param>
    void        Lerp(const NiVec2& _begin, const NiVec2& _end, float _t);

    /// ==========
    /// Minus sign
    /// ==========

    NiVec2     operator-() const;

    /// =====
    /// float
    /// =====

    NiVec2     operator+(float) = delete;
    NiVec2     operator-(float) = delete;
    NiVec2     operator*(float _f) const;
    NiVec2     operator/(float _f) const;
    NiVec2&    operator+=(float _f) = delete;
    NiVec2&    operator-=(float _f) = delete;
    NiVec2&    operator*=(float _f);
    NiVec2&    operator/=(float _f);

    /// =======
    /// NiVec2
    /// =======

    NiVec2     operator+(const NiVec2& _v) const;
    NiVec2     operator-(const NiVec2& _v) const;
    NiVec2     operator*(const NiVec2& _v) const;

    NiVec2&    operator+=(const NiVec2& _v);
    NiVec2&    operator-=(const NiVec2& _v);
    NiVec2&    operator*=(const NiVec2& _v);

    bool        operator==(const NiVec2& _v) const;
};

NiVec2 operator*(const float _f, const NiVec2& _v);
NiVec2 operator/(const float _f, const NiVec2& _v);