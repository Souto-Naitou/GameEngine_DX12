#include "../Matrix3x3.h"

#include "../Vector2.h"

#include <cmath>

Matrix3x3 Matrix3x3::operator -() const
{
    Matrix3x3 result = {};

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            result.m[i][j] = -m[i][j];
        }
    }

    return result;
}

Matrix3x3 Matrix3x3::Identity()
{
    Matrix3x3 result = {};
    result.m[0][0] = 1.0f;
    result.m[0][1] = 0.0f;
    result.m[0][2] = 0.0f;
    result.m[1][0] = 0.0f;
    result.m[1][1] = 1.0f;
    result.m[1][2] = 0.0f;
    result.m[2][0] = 0.0f;
    result.m[2][1] = 0.0f;
    result.m[2][2] = 1.0f;

    return result;
}

Matrix3x3 Matrix3x3::RotateMatrix(float _radian)
{
    Matrix3x3 result = {};
    result.m[0][0] = std::cosf(_radian);
    result.m[0][1] = -std::sinf(_radian);
    result.m[0][2] = 0.0f;
    result.m[1][0] = std::sinf(_radian);
    result.m[1][1] = std::cosf(_radian);
    result.m[1][2] = 0.0f;
    result.m[2][0] = 0.0f;
    result.m[2][1] = 0.0f;
    result.m[2][2] = 1.0f;

    return result;
}

Matrix3x3 Matrix3x3::ScaleMatrix(const Vector2& _scale)
{
    Matrix3x3 result = {};
    result.m[0][0] = _scale.x;
    result.m[0][1] = 0.0f;
    result.m[0][2] = 0.0f;
    result.m[1][0] = 0.0f;
    result.m[1][1] = _scale.y;
    result.m[1][2] = 0.0f;
    result.m[2][0] = 0.0f;
    result.m[2][1] = 0.0f;
    result.m[2][2] = 1.0f;

    return result;
}

Matrix3x3 Matrix3x3::TranslateMatrix(const Vector2& _translate)
{
    Matrix3x3 result = {};
    result.m[0][0] = 1.0f;
    result.m[0][1] = 0.0f;
    result.m[0][2] = 0.0f;
    result.m[1][0] = 0.0f;
    result.m[1][1] = 1.0f;
    result.m[1][2] = 0.0f;
    result.m[2][0] = _translate.x;
    result.m[2][1] = _translate.y;
    result.m[2][2] = 1.0f;

    return result;
}

Matrix3x3& Matrix3x3::operator +=(const Matrix3x3& _rm)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            m[i][j] += _rm.m[i][j];
        }
    }

    return *this;
}

Matrix3x3& Matrix3x3::operator -=(const Matrix3x3& _rm)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            m[i][j] -= _rm.m[i][j];
        }
    }

    return *this;
}

Matrix3x3& Matrix3x3::operator *=(const Matrix3x3& _rm)
{
    for (int off = 0; off < 3; off++)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                m[off][i] += m[off][j] * _rm.m[j][i];
            }
        }
    }

    return *this;
}

Matrix3x3 Matrix3x3::operator +(const Matrix3x3& _rm) const
{
    Matrix3x3 result = {};

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            result.m[i][j] = m[i][j] + _rm.m[i][j];
        }
    }

    return result;
}

Matrix3x3 Matrix3x3::operator -(const Matrix3x3& _rm) const
{
    Matrix3x3 result = {};
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            result.m[i][j] = m[i][j] - _rm.m[i][j];
        }
    }

    return result;
}

Matrix3x3 Matrix3x3::operator *(const Matrix3x3& _rm) const
{
    Matrix3x3 result = {};

    for (int off = 0; off < 3; off++)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                result.m[off][i] += m[off][j] * _rm.m[j][i];
            }
        }
    }

    return result;
}

Matrix3x3& Matrix3x3::operator *=(float _f)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            m[i][j] *= _f;
        }
    }

    return *this;
}

Matrix3x3 Matrix3x3::operator *(float _f) const
{
    Matrix3x3 result = {};

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            result.m[i][j] = m[i][j] * _f;
        }
    }

    return result;
}

