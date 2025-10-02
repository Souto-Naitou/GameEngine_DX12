#include "Color.h"
#include <array>
#include <sstream>
#include <algorithm>

RGBA RGB::to_RGBA(uint8_t _a) const
{
    return RGBA{ r_, g_, b_, _a };
}

RGBA color::create(const std::string& _colorstr)
{
    bool isHex = false;
    bool isRGBA = false;
    bool isRGB = false;

    std::string prefix = _colorstr.substr(0, 4);

    if (prefix.front() == '#') { isHex = true; }
    else if (prefix == "rgba") { isRGBA = true; }
    else if (prefix == "rgb") { isRGB = true; }

    std::string payload = {};

    if (isHex)
    {
        // #FF0000
        //  ^^^^^^
        payload = _colorstr.substr(1);
        return _HexToRGBA(payload);
    }
    else if (isRGBA)
    {
        // rgba(255, 0, 0, 1)
        //      ^^^^^^^^^^^^
        payload = _colorstr.substr(5, _colorstr.size() - 6);
        return _RGBAToRGBA(payload);
    }
    else if (isRGB)
    {
        // rgb(255, 0, 0)
        //     ^^^^^^^^^
        payload = _colorstr.substr(4, _colorstr.size() - 5);
        return _RGBToRGBA(payload);
    }

    return {};
}

RGBA color::_HexToRGBA(const std::string& _hexstr)
{
    if (_hexstr.size() == 6)
    {
        // AABBCC
        return RGBA{ static_cast<uint8_t>(std::stoul(_hexstr.substr(0, 2), nullptr, 16)),
            static_cast<uint8_t>(std::stoul(_hexstr.substr(2, 2), nullptr, 16)),
            static_cast<uint8_t>(std::stoul(_hexstr.substr(4, 2), nullptr, 16)),
            255 };
    }
    else if (_hexstr.size() == 8)
    {
        // AABBCCDD
        return RGBA{ static_cast<uint8_t>(std::stoul(_hexstr.substr(0, 2), nullptr, 16)),
            static_cast<uint8_t>(std::stoul(_hexstr.substr(2, 2), nullptr, 16)),
            static_cast<uint8_t>(std::stoul(_hexstr.substr(4, 2), nullptr, 16)),
            static_cast<uint8_t>(std::stoul(_hexstr.substr(6, 2), nullptr, 16)) };
    }

    return {};
}

RGBA color::_RGBToRGBA(const std::string& _rgbstr)
{
    // _rgbstr == 255, 0, 0

    // カンマで分割
    RGBA result = {};
    std::array<uint8_t, 3> rgb = {};

    std::istringstream ss(_rgbstr);
    std::string token;
    size_t i = 0;
    while (std::getline(ss, token, ',') && i < 3)
    {
        rgb[i] = static_cast<uint8_t>(std::stoi(token));
        ++i;
    }

    result.r() = rgb[0];
    result.g() = rgb[1];
    result.b() = rgb[2];
    result.a() = 255;

    return result;
}

RGBA color::_RGBAToRGBA(const std::string& _rgbastr)
{
    // _rgbastr == 255, 0, 0, 1

    // カンマで分割
    RGBA result = {};
    std::array<uint8_t, 4> rgba = {};

    std::istringstream ss(_rgbastr);
    std::string token;
    size_t i = 0;
    while (std::getline(ss, token, ',') && i < 4)
    {
        rgba[i] = static_cast<uint8_t>(std::stoi(token));
        ++i;
    }

    result.r() = rgba[0];
    result.g() = rgba[1];
    result.b() = rgba[2];
    result.a() = rgba[3];

    return result;
}

RGB HSV::to_RGB() const
{
    float r, g, b;
    float h = h_ / 360.0f;
    float s = s_ / 100.0f;
    float v = v_ / 100.0f;

    if (s == 0.0f)
    {
        r = g = b = v; // achromatic
    }
    else
    {
        int i = static_cast<int>(h * 6);
        float f = h * 6 - i;
        float p = v * (1 - s);
        float q = v * (1 - f * s);
        float t = v * (1 - (1 - f) * s);
        i %= 6;
        switch (i)
        {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
        }
    }

    r = std::clamp(r, 0.0f, 1.0f);
    g = std::clamp(g, 0.0f, 1.0f);
    b = std::clamp(b, 0.0f, 1.0f);

    return RGB{ static_cast<uint8_t>(r * 255), static_cast<uint8_t>(g * 255), static_cast<uint8_t>(b * 255) };
}
