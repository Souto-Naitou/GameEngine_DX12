#pragma once
#include <vector>
#include <Matrix4x4.h>
#include <wrl/client.h>
#include <d3d12.h>
#include <span>
#include <array>

const uint32_t kNumMaxInfluence = 4;
struct VertexInfluence
{
    std::array<float, kNumMaxInfluence> weights;
    std::array<int32_t, kNumMaxInfluence> jointIndices;
};

struct WellForGPU
{
    Matrix4x4 skeletonSpaceMatrix = {}; // 位置用
    Matrix4x4 skeletonSpaceInverseTransposeMatrix = {}; // 法線用
};

struct SkinningInformation
{
    uint32_t numVertices;
};

struct SkinCluster
{
    std::vector<Matrix4x4>                  inverseBindPoseMatrices = {};
    D3D12_VERTEX_BUFFER_VIEW                influenceBufferView = {};
    Microsoft::WRL::ComPtr<ID3D12Resource>  resourceInfluence = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource>  resourcePalette = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource>  resourceSkinningInformation = nullptr;
    std::span<VertexInfluence>              mappedInfluences = {};
    std::span<WellForGPU>                   mappedPalette = {};
    std::span<SkinningInformation>          mappedSkinningInformation = {};
    uint32_t                                srvIndexPalette = 0;
    uint32_t                                srvIndexInfluence = 0;
    uint32_t                                srvIndexSkinningInformation = 0;
    std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> srvHandlePalette = {};
    std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> srvHandleInfluence = {};
};