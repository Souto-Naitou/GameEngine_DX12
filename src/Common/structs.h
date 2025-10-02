#pragma once
#include <Vector4.h>
#include <Vector3.h>
#include <Vector2.h>
#include <Matrix4x4.h>

#include <vector>
#include <string>
#include <dxgidebug.h>
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma warning(disable: 4324) // 構造体のメンバがアライメントのためにパディングされる


enum class LightingType : int32_t
{
    LambertianReflectance,
    HarfLambert
};

struct DirectionalLight
{
    Vector4 color; //!< ライトの色
    Vector3 direction; //!< ライトの向き (正規化必須)
    float intensity; //!< 輝度
};

struct TransformationMatrix
{
    Matrix4x4 wvp;
    Matrix4x4 world;
};

struct ParticleForGPU
{
    Matrix4x4 wvp;
    Matrix4x4 world;
    Vector4 color;
};

struct TilingData
{
    Vector2 tilingMultiply;
    int padding[2];
};