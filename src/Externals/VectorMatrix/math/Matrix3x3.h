#pragma once

/// 前方宣言
class Vector2;

/// <summary>
/// 3x3行列
/// </summary>
class Matrix3x3 final 
{
public:
    Matrix3x3() = default;
    ~Matrix3x3() = default;

    float m[3][3];

    /// ====================
    /// Minus Sign
    /// ====================
    
    Matrix3x3 operator-() const;

    /// =====================
    /// Make Matrix
    /// =====================
    
    static Matrix3x3 Identity();
    static Matrix3x3 RotateMatrix(float _radian);
    static Matrix3x3 ScaleMatrix(const Vector2& _scale);
    static Matrix3x3 TranslateMatrix(const Vector2& _translate);

    /// ====================
    /// Operator
    /// ====================
    
    Matrix3x3& operator +=(const Matrix3x3& _rm);
    Matrix3x3& operator -=(const Matrix3x3& _rm);
    Matrix3x3& operator *=(const Matrix3x3& _rm);

    Matrix3x3 operator +(const Matrix3x3& _rm) const;
    Matrix3x3 operator -(const Matrix3x3& _rm) const;
    Matrix3x3 operator *(const Matrix3x3& _rm) const;

    Matrix3x3& operator *=(float _f);
    Matrix3x3 operator *(float _f) const;
};