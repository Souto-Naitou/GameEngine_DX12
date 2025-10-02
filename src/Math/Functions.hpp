#pragma once

#include <cmath>
#include <numbers>

namespace Math
{
    inline float rcp(float _x)
    {
        return 1.0f / _x;
    }

    inline float gauss(float _x, float _y, float _sigma)
    {
        float exponent = -(_x * _x + _y * _y) * rcp(2.0f * _sigma * _sigma);
        float denominator = 2.0f * std::numbers::pi_v<float> * _sigma * _sigma;
        return std:: exp(exponent) * rcp(denominator);
    }
}