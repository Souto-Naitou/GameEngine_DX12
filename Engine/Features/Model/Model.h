#pragma once

#include <Common/structs.h>
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <Core/DirectX12/DirectX12.h>
#include <thread>
#include <memory>


struct Material;
struct VertexData;

class Model
{
public: /// 公開メンバ関数
    void Initialize(const std::string& _filePath);
    void Update();
    void Draw();
    ~Model();


public: /// Getter
    ModelData* GetModelData() { return &modelData_; }
    D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const { return vertexBufferView_; }
    D3D12_GPU_DESCRIPTOR_HANDLE GetTextureSrvHandleGPU() const { return textureSrvHandleGPU_; }


private: /// メンバ変数
    static const std::string kDefaultDirectoryPath;

    ModelData                               modelData_                  = {};
    Microsoft::WRL::ComPtr<ID3D12Resource>  vertexResource_             = nullptr;
    D3D12_VERTEX_BUFFER_VIEW                vertexBufferView_           = {};
    VertexData*                             vertexData_                 = nullptr;
    D3D12_GPU_DESCRIPTOR_HANDLE             textureSrvHandleGPU_        = {};



private: /// 非公開メンバ関数
    void CreateVertexResource();
    void LoadModelTexture();

private: /// 他クラスのインスタンス
    DirectX12* pDx12_ = nullptr;
    ID3D12Device* device_ = nullptr;
    std::string filePath_;
    std::string directoryPath_;
};