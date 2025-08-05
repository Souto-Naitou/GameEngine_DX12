#pragma once

#include <Core/DirectX12/DirectX12.h>
#include <Vector4.h>
#include <array>
#include <cstdint>
#include <Common/structs.h>
#include <Features/GameEye/GameEye.h>
#include "CubemapSystem.h"
#include <Math/Transform.h>

class Skybox
{
public:
    Skybox() = default;
    ~Skybox() = default;
    void Initialize(CubemapSystem* _cms);
    void Finalize() const;
    void Update();
    void Draw() const;
    void SetSkyboxTexture(D3D12_GPU_DESCRIPTOR_HANDLE _handle);

private:
    void _CreateVertices(std::array<Vector4, 24>& _out_vertices);
    void _CreateIndices(std::array<uint32_t, 36>& _out_indices);
    void _CreateVertexResource();
    void _CreateIndexResource();
    void _CreateTransformationMatrixResource();
    void _CreateMaterialResource();
    void _ImGui();

    Microsoft::WRL::ComPtr<ID3D12Resource>  vertexResource_         = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource>  indexResource_          = nullptr;  //< インデックスバッファリソース
    D3D12_VERTEX_BUFFER_VIEW                vertexBufferView_       = {};
    D3D12_INDEX_BUFFER_VIEW                 indexBufferView_        = {};
    Vector4*                                mappedVertices_         = nullptr;  //< 頂点データのポインタ
    uint32_t*                               mappedIndices_          = nullptr;  //< インデックスデータのポインタ
    EulerTransform                          transformation_         = {};  //< Skyboxの変形情報 
    std::string                             name_                   = "Skybox";

    D3D12_GPU_DESCRIPTOR_HANDLE             skyboxTextureSrvHandleGpu_ = {};
    
    Microsoft::WRL::ComPtr<ID3D12Resource> resourceTransformationMatrix_ = nullptr;
    TransformationMatrix* mappedTransformationMatrix_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource> resourceMaterial_ = nullptr;
    Vector4* mappedMaterial_ = nullptr;

    GameEye** ppGlobalEye_ = nullptr;  //< ゲームカメラ

    // Pointers
    DirectX12* pDx12_ = nullptr;  //< DirectX12へのポインタ
    ID3D12Device* device_ = nullptr;
    CubemapSystem* pCubemapSystem_ = nullptr;  //< キューブマップシステムへのポインタ
};