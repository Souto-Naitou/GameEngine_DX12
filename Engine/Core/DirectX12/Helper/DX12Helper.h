#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <stdint.h>
#include <dxcapi.h>
#include <string>
#include <DirectXTex/DirectXTex.h>
#include <vector>
#include <Vector4.h>

namespace DX12Helper
{
    using Microsoft::WRL::ComPtr;

    /// <summary>
    /// デバイスを生成する
    /// </summary>
    /// <param name="_device">生成先</param>
    /// <param name="_adapter">使用するアダプタ</param>
    void CreateDevice(ComPtr<ID3D12Device>& _device, ComPtr<IDXGIAdapter4>& _adapter);


    #ifdef _DEBUG
    /// <summary>
    /// エラー時に停止する
    /// </summary>
    /// <param name="_device">デバイス</param>
    /// <param name="_infoQ">情報キュー</param>
    void PauseError(ComPtr<ID3D12Device>& _device, ComPtr<ID3D12InfoQueue>& _infoQ);
    #endif // _DEBUG


    /// <summary>
    /// ディスクリプタヒープの生成
    /// </summary>
    /// <param name="_device">デバイス</param>
    /// <param name="_heapType">ヒープタイプ</param>
    /// <param name="_numDescriptors">ディスクリプタの数</param>
    /// <param name="_shaderVisible">シェーダーが参照可能か</param>
    /// <returns></returns>
    ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
        const ComPtr<ID3D12Device>& _device,
        D3D12_DESCRIPTOR_HEAP_TYPE _heapType,
        UINT _numDescriptors,
        bool _shaderVisible);


    /// <summary>
    /// DepthStencilTextureリソースを生成
    /// </summary>
    /// <param name="_device">デバイス</param>
    /// <param name="_width">幅</param>
    /// <param name="_height">高さ</param>
    /// <returns>リソース</returns>
    ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(const ComPtr<ID3D12Device>& _device, int32_t _width, int32_t _height);


    /// <summary>
    /// シェーダーをコンパイル
    /// </summary>
    /// <param name="filePath">ファイルパス</param>
    /// <param name="profile">プロファイル</param>
    /// <param name="dxcUtils">DXCユーティリティ</param>
    /// <param name="dxcCompiler">DXCコンパイラ</param>
    /// <param name="includeHandler">インクルードハンドラ</param>
    /// <returns>コンパイル結果</returns>
    ComPtr<IDxcBlob> CompileShader(
        const std::wstring& filePath,
        const wchar_t* profile,
        const ComPtr<IDxcUtils>& dxcUtils,
        const ComPtr<IDxcCompiler3>& dxcCompiler,
        const ComPtr<IDxcIncludeHandler>& includeHandler);


    /// <summary>
    /// バッファリソースを生成
    /// </summary>
    /// <param name="_device">デバイス</param>
    /// <param name="_sizeInBytes">バイト数</param>
    /// <returns>リソース</returns>
    ComPtr<ID3D12Resource> CreateBufferResource(const ComPtr<ID3D12Device>& _device, size_t _sizeInBytes);


    /// <summary>
    /// テクスチャリソースを生成
    /// </summary>
    /// <param name="_filePath">ファイルパス</param>
    DirectX::ScratchImage LoadTexture(const std::string _filePath);

    ComPtr<ID3D12Resource> CreateTextureResource(const ComPtr<ID3D12Device>& _device, const DirectX::TexMetadata& _metadata);
    void UploadTextureData(const ComPtr<ID3D12Resource>& _texture, const DirectX::ScratchImage& _mipImages);
    D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(const ComPtr<ID3D12DescriptorHeap>& _descriptorHeap, uint32_t _descriptorSize, uint32_t _index);
    D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(const ComPtr<ID3D12DescriptorHeap>& _descriptorHeap, uint32_t _descriptorSize, uint32_t _index);


    /// <summary>
    /// テクスチャリソースを生成
    /// </summary>
    /// <param name="_device">デバイス</param>
    /// <param name="_srvDescriptorHeap">SRVディスクリプタヒープ</param>
    /// <param name="_kDescriptorSizeSRV">SRVディスクリプタサイズ</param>
    /// <param name="_path">ファイルパス</param>
    /// <param name="_textureResources">テクスチャリソース</param>
    /// <returns></returns>
    void CreateNewTexture(const ComPtr<ID3D12Device>& _device,
        const ComPtr<ID3D12DescriptorHeap>& _srvDescriptorHeap,
        const uint32_t _kDescriptorSizeSRV,
        const char* _path,
        std::vector<ComPtr<ID3D12Resource>>& _textureResources);

    /// <summary>
    /// 頂点リソースを生成
    /// </summary>
    /// <param name="_device">デバイス</param>
    ComPtr<ID3D12Resource> CreateVertexResource(const ComPtr<ID3D12Device>& _device, unsigned int _countVertex);

    void ChangeStateResource(
        const ComPtr<ID3D12GraphicsCommandList>& _commandList, 
        const ComPtr<ID3D12Resource>& _resource, 
        D3D12_RESOURCE_STATES _before, 
        D3D12_RESOURCE_STATES _after
    );

    void CommandListCommonSetting(ID3D12GraphicsCommandList* _commandList, const D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandle);

    /// <summary>
    /// レンダーターゲットテクスチャリソースを生成
    /// </summary>
    /// <param name="_device">デバイス</param>
    /// <param name="_width">幅</param>
    /// <param name="_height">高さ</param>
    /// <param name="_format">フォーマット</param>
    /// <param name="_clearColor">クリア色</param>
    /// <returns>レンダーテクスチャリソース</returns>
    ComPtr<ID3D12Resource> CreateRenderTextureResource(
        const ComPtr<ID3D12Device>& _device,
        int32_t _width,
        int32_t _height,
        DXGI_FORMAT _format,
        const Vector4& _clearColor
    );
}