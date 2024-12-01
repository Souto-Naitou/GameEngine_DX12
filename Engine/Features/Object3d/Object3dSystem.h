#pragma once

#include <Vector4.h>
#include <Common/structs.h>
#include <Core/DirectX12/DirectX12.h>
#include <Features/GameEye/GameEye.h>

struct Material
{
    Vector4 color;
    LightingType lightingType;
    int32_t enableLighting;
    float padding[2];
    Matrix4x4 uvTransform;
};

/// <summary>
///  3Dオブジェクトシステム
/// </summary>
class Object3dSystem
{
public:
    Object3dSystem();

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize();

    void PresentDraw();

public: /// Getter
    DirectX12* GetDx12()                        { return pDx12_; }
    GameEye* GetDefaultGameEye()                { return pDefaultGameEye_; }

public: /// Setter
    void SetDefaultGameEye(GameEye* _pGameEye)  { pDefaultGameEye_ = _pGameEye; }

private:
    void CreateRootSignature();
    void CreatePipelineState();

private: /// メンバ変数
    static constexpr wchar_t kVertexShaderPath[] = L"EngineResources/Shaders/Object3d.VS.hlsl";
    static constexpr wchar_t kPixelShaderPath[] = L"EngineResources/Shaders/Object3d.PS.hlsl";
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;


private:
    DirectX12*  pDx12_              = nullptr;
    GameEye*    pDefaultGameEye_    = nullptr;
};