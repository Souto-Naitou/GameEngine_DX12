#pragma once

#include "./IModel.h"

#include <Common/structs.h>
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <Core/DirectX12/DirectX12.h>
#include <memory>

struct Material;
struct VertexData;

class ObjModel : public IModel
{
public:
    // Common functions
    ~ObjModel()                                     override = default;
    void    Initialize()                            override;
    void    Finalize()                              override {};
    void    Update()                                override;
    void    Draw(ID3D12GraphicsCommandList* _cl)    override;
    void    CreateGPUResource()                     override;
    void    Clone(IModel* _src)                     override;
    std::unique_ptr<IModel> Cloned()                override;

    // Getter
    ModelData*                  GetModelData()                  override;
    size_t                      GetVertexCount()        const   override;
    D3D12_VERTEX_BUFFER_VIEW    GetVertexBufferView()   const   override;
    D3D12_GPU_DESCRIPTOR_HANDLE GetTextureSrvHandle()   const   override;
    bool                        IsEndLoading()          const   override;

    // Setter
    void ChangeTexture(D3D12_GPU_DESCRIPTOR_HANDLE _texSrvHnd) override;


private: /// メンバ変数
    static const std::string kDefaultDirectoryPath;

    ModelData                               modelData_                  = {};
    Microsoft::WRL::ComPtr<ID3D12Resource>  vertexResource_             = nullptr;
    D3D12_VERTEX_BUFFER_VIEW                vertexBufferView_           = {};
    VertexData*                             vertexData_                 = nullptr;
    D3D12_GPU_DESCRIPTOR_HANDLE             textureSrvHandleGPU_        = {};
    bool                                    isReadyDraw_                = false;
    ObjModel*                               pCloneSrc_                  = nullptr;  //< クローン元のインスタンス
    bool                                    isOverwroteTexture_         = false;    //< テクスチャを上書きしたかどうか

private: /// 非公開メンバ関数
    void _CreateVertexResource();
    void _LoadModelTexture();
    void _CopyFrom(ObjModel* pCopySrc);
};