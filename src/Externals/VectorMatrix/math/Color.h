#pragma once
#include <cstdint>
#include <string>

#include <Vector4.h>
#include <Vector3.h>

#undef RGB
#undef RGBA

class IColor
{
public:
    virtual ~IColor() = default;
};

class RGBA;

class RGB : public IColor
{
public:
    RGB() = default;
    RGB(uint8_t _r, uint8_t _g, uint8_t _b) : r_(_r), g_(_g), b_(_b) {}
    RGB(const RGB& _other) : r_(_other.r_), g_(_other.g_), b_(_other.b_) {}
    RGB(RGB&& _other) : r_(_other.r_), g_(_other.g_), b_(_other.b_) {}

    uint8_t& r() { return r_; }
    uint8_t& g() { return g_; }
    uint8_t& b() { return b_; }

    RGB& operator=(const RGB& other) { r_ = other.r_; g_ = other.g_; b_ = other.b_; return *this; }
    RGB& operator=(RGB&& other) { r_ = other.r_; g_ = other.g_; b_ = other.b_; return *this; }
    RGB& operator=(const RGBA& other);
    RGB& operator=(RGBA&& other);

    RGB& operator=(const Vector3& other)
    {
        r_ = static_cast<uint8_t>(other.x * 255);
        g_ = static_cast<uint8_t>(other.y * 255);
        b_ = static_cast<uint8_t>(other.z * 255);
        return *this;
    }

    RGB& operator=(Vector3&& other)
    {
        r_ = static_cast<uint8_t>(other.x * 255);
        g_ = static_cast<uint8_t>(other.y * 255);
        b_ = static_cast<uint8_t>(other.z * 255);
        return *this;
    }

    RGB& operator=(const Vector4& other)
    {
        r_ = static_cast<uint8_t>(other.x * 255);
        g_ = static_cast<uint8_t>(other.y * 255);
        b_ = static_cast<uint8_t>(other.z * 255);
        return *this;
    }

    RGB& operator=(Vector4&& other)
    {
        r_ = static_cast<uint8_t>(other.x * 255);
        g_ = static_cast<uint8_t>(other.y * 255);
        b_ = static_cast<uint8_t>(other.z * 255);
        return *this;
    }

    ~RGB() = default;

    RGBA to_RGBA(uint8_t _a = 255) const;
    Vector4 to_Vector4(uint8_t _a) const { return Vector4(r_ / 255.0f, g_ / 255.0f, b_ / 255.0f, _a / 255.0f); }
    Vector4 to_Vector4(float _a) const { return Vector4(r_ / 255.0f, g_ / 255.0f, b_ / 255.0f, _a); }
    Vector3 to_Vector3() const { return Vector3(r_ / 255.0f, g_ / 255.0f, b_ / 255.0f); }


private:
    uint8_t r_ = 255;
    uint8_t g_ = 255;
    uint8_t b_ = 255;
};

class RGBA : public IColor
{
public:
    RGBA() = default;

    uint8_t& r() { return r_; }
    uint8_t& g() { return g_; }
    uint8_t& b() { return b_; }
    uint8_t& a() { return a_; }

    RGBA(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) : r_(_r), g_(_g), b_(_b), a_(_a) {}
    RGBA(const RGBA& _other) : r_(_other.r_), g_(_other.g_), b_(_other.b_), a_(_other.a_) {}
    RGBA(uint32_t _rgba) : r_((_rgba >> 24) & 0xFF), g_((_rgba >> 16) & 0xFF), b_((_rgba >> 8) & 0xFF), a_(_rgba & 0xFF) {}
    RGBA(RGBA&& _other) : r_(_other.r_), g_(_other.g_), b_(_other.b_), a_(_other.a_) {}

    RGBA& operator=(const RGBA& other)
    {
        r_ = other.r_;
        g_ = other.g_;
        b_ = other.b_;
        a_ = other.a_;
        return *this;
    }

    RGBA& operator=(RGBA&& other)
    {
        r_ = other.r_;
        g_ = other.g_;
        b_ = other.b_;
        a_ = other.a_;
        return *this;
    }

    RGBA& operator=(const Vector4& other)
    {
        r_ = static_cast<uint8_t>(other.x * 255);
        g_ = static_cast<uint8_t>(other.y * 255);
        b_ = static_cast<uint8_t>(other.z * 255);
        a_ = static_cast<uint8_t>(other.w * 255);
        return *this;
    }

    RGBA& operator=(Vector4&& other)
    {
        r_ = static_cast<uint8_t>(other.x * 255);
        g_ = static_cast<uint8_t>(other.y * 255);
        b_ = static_cast<uint8_t>(other.z * 255);
        a_ = static_cast<uint8_t>(other.w * 255);
        return *this;
    }

    RGB rgb() const { return RGB(r_, g_, b_); }

    Vector4 to_Vector4() const { return Vector4(r_ / 255.0f, g_ / 255.0f, b_ / 255.0f, a_ / 255.0f); }
    Vector3 to_Vector3() const { return Vector3(r_ / 255.0f, g_ / 255.0f, b_ / 255.0f); }


private:
    uint8_t r_ = 255;
    uint8_t g_ = 255;
    uint8_t b_ = 255;
    uint8_t a_ = 255;
};

class HSV : public IColor
{
public:

    HSV() = default;
    HSV(float _h, float _s, float _v) : h_(_h), s_(_s), v_(_v) {}
    HSV(const HSV& _other) : h_(_other.h_), s_(_other.s_), v_(_other.v_) {}
    HSV(HSV&& _other) noexcept : h_(_other.h_), s_(_other.s_), v_(_other.v_) {}

    float& h() { return h_; }
    float& s() { return s_; }
    float& v() { return v_; }

    const float& h() const { return h_; }
    const float& s() const { return s_; }
    const float& v() const { return v_; }

    RGB to_RGB() const;

private:
    float h_ = 0.0f; // Hue [0, 360)
    float s_ = 1.0f; // Saturation [0, 1]
    float v_ = 1.0f; // Value [0, 1]
};

namespace color
{
    RGBA create(const std::string& _colorstr);
    RGBA _HexToRGBA(const std::string& _hexstr);
    RGBA _RGBToRGBA(const std::string& _rgbstr);
    RGBA _RGBAToRGBA(const std::string& _rgbastr);
}