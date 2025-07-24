#include "DX12Helper.h"

#pragma warning(disable: 6387) // 例外の可能性がある関数の警告を無視

#include <cassert>
#include <DebugTools/Logger/Logger.h>
#include <format>
#include <intsafe.h>
#include <Utility/ConvertString/ConvertString.h>
#include <Core/DirectX12/DirectX12.h>
#include <Features/Model/ModelData.h>
#include <Core/DirectX12/SRVManager.h>

void DX12Helper::CreateDevice(Microsoft::WRL::ComPtr<ID3D12Device>& _device, Microsoft::WRL::ComPtr<IDXGIAdapter4>& _adapter)
{
    /// D3D12Deviceの生成

    // 機能レベルとログ出力用の文字列
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_12_2, D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0
    };
    const char* featureLevelStrings[] = { "12.2", "12.1", "12.0" };
    // 高い順に生成できるか試していく
    for (size_t i = 0; i < _countof(featureLevels); ++i)
    {
        // 採用したアダプターでデバイスを生成
        HRESULT hr = D3D12CreateDevice(_adapter.Get(), featureLevels[i], IID_PPV_ARGS(_device.GetAddressOf()));
        // 指定した機能レベルでデバイスが生成できたかを確認
        if (SUCCEEDED(hr))
        {
            // 生成できたのでログ出力を行ってループを抜ける
            Logger::GetInstance()->LogInfo(__FILE__, __FUNCTION__, std::format("FeatureLevel : {}", featureLevelStrings[i]));
            break;
        }
    }
    // デバイスの生成がうまくいかなかったので起動できない
    if (!_device)
    {
        Logger::GetInstance()->LogError(__FILE__, __FUNCTION__, "Failed to create");
        assert(_device && "Failed to create device");
    }

    Logger::GetInstance()->LogInfo(__FILE__, __FUNCTION__, "Initilization succeeded"); // 初期化完了のログを出力
}


//#ifdef _DEBUG
void DX12Helper::PauseError(Microsoft::WRL::ComPtr<ID3D12Device>& _device, Microsoft::WRL::ComPtr<ID3D12InfoQueue>& _infoQ)
{

    if (SUCCEEDED(_device->QueryInterface(IID_PPV_ARGS(_infoQ.GetAddressOf()))))
    {
        // やばいエラー時に止まる
        _infoQ->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
        // エラー時に止まる <- 解放忘れが判明したら、コメントアウト
        _infoQ->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
        //警告時に止まる
        _infoQ->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

        // 抑制するメッセージのID
        D3D12_MESSAGE_ID denyIds[] = {
            // Windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤーの相互作用バグによるエラーメッセージ
            D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE,
            D3D12_MESSAGE_ID_CREATERESOURCE_STATE_IGNORED
        };

        // 抑制するレベル
        D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
        D3D12_INFO_QUEUE_FILTER filter{};
        filter.DenyList.NumIDs        = _countof(denyIds);
        filter.DenyList.pIDList       = denyIds;
        filter.DenyList.NumSeverities = _countof(severities);
        filter.DenyList.pSeverityList = severities;
        // 指定したメッセージの表示を制限する
        _infoQ->PushStorageFilter(&filter);
    }

}
//#endif // _DEBUG

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DX12Helper::CreateDescriptorHeap(const Microsoft::WRL::ComPtr<ID3D12Device>& _device, D3D12_DESCRIPTOR_HEAP_TYPE _heapType, UINT _numDescriptors, bool _shaderVisible)
{
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;
    D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
    descriptorHeapDesc.Type           = _heapType;
    descriptorHeapDesc.NumDescriptors = _numDescriptors;
    descriptorHeapDesc.Flags          = _shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    HRESULT hr = _device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));

    if (FAILED(hr))
    {
        Logger::GetInstance()->LogError(
            "DX12Helper",
            __func__,
            "Failed to create descriptor heap."
        );
        assert(false && "Failed to create descriptor heap");
    }

    return descriptorHeap;
}

Microsoft::WRL::ComPtr<ID3D12Resource> DX12Helper::CreateDepthStencilTextureResource(const Microsoft::WRL::ComPtr<ID3D12Device>& _device, int32_t _width, int32_t _height)
{
    D3D12_RESOURCE_DESC resourceDesc{};
    resourceDesc.Width            = _width;                                     // 幅
    resourceDesc.Height           = _height;                                    // 高さ
    resourceDesc.MipLevels        = 1;                                          // mipmapの数
    resourceDesc.DepthOrArraySize = 1;	                                        // 奥行き or 配列Textureの配列数
    resourceDesc.Format           = DXGI_FORMAT_D24_UNORM_S8_UINT;              // フォーマット
    resourceDesc.SampleDesc.Count = 1;                                          // サンプリング数
    resourceDesc.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D;         // 2DTexture
    resourceDesc.Flags            = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;    // DepthStencilとして使う通知


    /// 利用するHeapの設定
    D3D12_HEAP_PROPERTIES heapProperties{};
    heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;                  // VRAMに


    /// 深度値のクリア設定
    D3D12_CLEAR_VALUE depthClearValue{};
    depthClearValue.DepthStencil.Depth = 1.0f;                          // 1.0f(最大値)でクリア
    depthClearValue.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT; // フォーマット。Resourceと合わせる


    /// Resourceの生成
    Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
    HRESULT hr = _device->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &depthClearValue,
        IID_PPV_ARGS(&resource)
    );

    if (FAILED(hr))
    {
        Logger::GetInstance()->LogError(__FILE__, __FUNCTION__, "Failed to Create DSTR");
        assert(false && "Failed create depth stencil texture resource");
    }


    return resource;
}

Microsoft::WRL::ComPtr<IDxcBlob> DX12Helper::CompileShader(
    const std::wstring& filePath,
    const wchar_t* profile,
    const Microsoft::WRL::ComPtr<IDxcUtils>& dxcUtils,
    const Microsoft::WRL::ComPtr<IDxcCompiler3>& dxcCompiler,
    const Microsoft::WRL::ComPtr<IDxcIncludeHandler>& includeHandler
)
{
    /// 1. hlslファイルを読み込む

    // これからシェーダーをコンパイルする旨をログに出す
    std::string filepathA = ConvertString(filePath);
    std::string profileA = ConvertString(profile);
    Logger::GetInstance()->LogInfo(
        __FILE__,
        __FUNCTION__,
        std::format("Begin compile. Path:{}, Profile:{}", filepathA, profileA)
    );

    // hlslファイルを読む
    Microsoft::WRL::ComPtr<IDxcBlobEncoding> shaderSource = nullptr;
    HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);

    // 読めなかったら止める
    if (FAILED(hr))
    {
        Logger::GetInstance()->LogError(__FILE__, __FUNCTION__, "Failed load file");

        assert(false && "Failed loadfile.");
    }

    // 読み込んだファイルの内容を設定する
    DxcBuffer shaderSourceBuffer;
    shaderSourceBuffer.Ptr      = shaderSource->GetBufferPointer();
    shaderSourceBuffer.Size     = shaderSource->GetBufferSize();
    shaderSourceBuffer.Encoding = DXC_CP_UTF8; // UTF-8の文字コードであることを通知

    /// 2. Compileする
    #ifdef _DEBUG
    LPCWSTR arguments[] = {
        filePath.c_str(),           // コンパイル対象のhlslファイル名
        L"-E", L"main",             // エントリーポイントの指定。基本的にmain以外にはしない
        L"-T", profile,             // ShaderProfileの設定
        L"-Zi", L"-Qembed_debug",   // デバッグ用の情報を埋め込む
        L"-Od",                     // 最適化を外しておく
        L"-Zpr",                    // メモリレイアウトは行優先
    };
    #else
    LPCWSTR arguments[] = {
        filePath.c_str(),           // コンパイル対象のhlslファイル名
        L"-E", L"main",             // エントリーポイントの指定。基本的にmain以外にはしない
        L"-T", profile,             // ShaderProfileの設定
        L"-Zi", L"-Qembed_debug",   // デバッグ用の情報を埋め込む
        L"-O3",                     // 最適化を有効に
        L"-Zpr",                    // メモリレイアウトは行優先
    };
    #endif

    // 実際にShaderをコンパイルする
    Microsoft::WRL::ComPtr<IDxcResult> shaderResult = nullptr;
    hr = dxcCompiler->Compile(
        &shaderSourceBuffer,        // 読み込んだファイル
        arguments,                  // コンパイルオプション
        _countof(arguments),        // コンパイルオプションの数
        includeHandler.Get(),       // includeが含まれた諸々
        IID_PPV_ARGS(&shaderResult) // コンパイル結果
    );

    // コンパイルエラーではなくdxcが起動できないなど致命的な状況
    if (FAILED(hr))
    {
        Logger::GetInstance()->LogError(__FILE__, __FUNCTION__, "Failed launch DXC compiler");
        assert(false && "Failed compile shader");
    }

    /// 3. 警告・エラーが出ていないか確認する
    Microsoft::WRL::ComPtr<IDxcBlobUtf8> shaderError = nullptr;
    shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
    if (shaderError != nullptr && shaderError->GetStringLength() != 0)
    {
        Logger::GetInstance()->LogError(__FILE__, __FUNCTION__, shaderError->GetStringPointer());
        assert(false);
    }

    /// 4. Compile結果を受け取って返す

    // コンパイル結果から実行用のバイナリ部分を取得
    Microsoft::WRL::ComPtr<IDxcBlob> shaderBlob = nullptr;
    hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
    assert(SUCCEEDED(hr));

    // 成功したログを出す
    Logger::GetInstance()->LogInfo(
        __FILE__,
        __FUNCTION__,
        "Compile succeeded." + std::format(" Size: {}bytes", shaderBlob->GetBufferSize())
    );

    // 実行用のバイナリを返却
    return shaderBlob;
}

Microsoft::WRL::ComPtr<ID3D12Resource> DX12Helper::CreateBufferResource(const Microsoft::WRL::ComPtr<ID3D12Device>& _device, size_t _sizeInBytes)
{
    return CreateBufferResource(_device, _sizeInBytes, D3D12_RESOURCE_FLAG_NONE);
}

Microsoft::WRL::ComPtr<ID3D12Resource> DX12Helper::CreateBufferResource(const ComPtr<ID3D12Device>& _device, size_t _sizeInBytes, D3D12_RESOURCE_FLAGS _flag)
{
    Microsoft::WRL::ComPtr<ID3D12Resource> result = nullptr;
    D3D12_HEAP_PROPERTIES uploadHeapProperties{};
    if (_flag & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS)
    {
        uploadHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
    }
    else
    {
        uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
    }

    D3D12_RESOURCE_DESC desc{};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Width = _sizeInBytes;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.SampleDesc.Count = 1;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = _flag;
    HRESULT hr = _device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
        &desc, D3D12_RESOURCE_STATE_COMMON, nullptr,
        IID_PPV_ARGS(&result));

    if (FAILED(hr))
    {
        Logger::GetInstance()->LogError(__FILE__, __FUNCTION__, "CreateCommittedResource failed");
        assert(false && "Create committed resource failed");
    }

    return result;
}

DirectX::ScratchImage DX12Helper::LoadTexture(const std::string& _filePath)
{
    DirectX::ScratchImage image{};
    std::wstring filePathW = ConvertString(_filePath);
    HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
    if (FAILED(hr))
    {
        Logger::GetInstance()->LogError(__FILE__, __FUNCTION__, "Failed LoadFromWICFile");
        assert(false && "Failed LoadFromWICFile");
    }

    DirectX::ScratchImage mipImages{};
    hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
    if (FAILED(hr))
    {
        Logger::GetInstance()->LogError(__FILE__, __FUNCTION__, "Failed GenerateMipMaps");
        assert(false && "Failed GenerateMipMaps");
    }

    return mipImages;
}

Microsoft::WRL::ComPtr<ID3D12Resource> DX12Helper::CreateTextureResource(const Microsoft::WRL::ComPtr<ID3D12Device>& _device, const DirectX::TexMetadata& _metadata)
{
    // metadataをもとにResourceの設定
    D3D12_RESOURCE_DESC resourceDesc{};
    resourceDesc.Width = UINT(_metadata.width);
    resourceDesc.Height = UINT(_metadata.height);
    resourceDesc.MipLevels = UINT16(_metadata.mipLevels);
    resourceDesc.DepthOrArraySize = UINT16(_metadata.arraySize);
    resourceDesc.Format = _metadata.format;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(_metadata.dimension);

    // 利用するHeapの設定。非常に特殊な運用。 02_04exで一般的なケース版がある
    D3D12_HEAP_PROPERTIES heapProperties{};
    heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
    heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

    // Resourceの生成
    Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
    HRESULT hr = _device->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_COMMON,
        nullptr,
        IID_PPV_ARGS(&resource));

    if (FAILED(hr))
    {
        Logger::GetInstance()->LogError(__FILE__, __FUNCTION__, "CreateCommittedResource failed");
        assert(false && "Create committed resource failed");
    }


    return resource;
}

void DX12Helper::UploadTextureData(const Microsoft::WRL::ComPtr<ID3D12Resource>& _texture, const DirectX::ScratchImage& _mipImages)
{
    // Meta情報を取得
    const DirectX::TexMetadata& metadata = _mipImages.GetMetadata();
    // 全MipMapについて
    for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel)
    {
        // MipMapLevelを指定して各Imageを取得
        const DirectX::Image* img = _mipImages.GetImage(mipLevel, 0, 0);
        // Textureに転送
        HRESULT hr = _texture->WriteToSubresource(
            UINT(mipLevel),
            nullptr,
            img->pixels,
            UINT(img->rowPitch),
            UINT(img->slicePitch)
        );
        if (FAILED(hr))
        {
            Logger::GetInstance()->LogError(__FILE__, __FUNCTION__, "WriteToSubresource failed");
            assert(false && "WriteToSubresource failed");
        }
    }

    return;
}

void DX12Helper::CreateNewTexture(DirectX12* _pDx12,
    const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& _srvDescriptorHeap,
    const uint32_t _kDescriptorSizeSRV,
    const char* _path,
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& _textureResources)
{
    auto device = _pDx12->GetDevice();
    int32_t numUploadedTexture = _pDx12->GetNumUploadedTexture();
    std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& textureSrvHandleCPUs = _pDx12->GetSRVHandlesCPUList();
    std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>& textureSrvHandleGPUs = _pDx12->GetSRVHandlesGPUList();

    DirectX::ScratchImage mipImage = LoadTexture(_path);
    const DirectX::TexMetadata& metadata = mipImage.GetMetadata();
    Microsoft::WRL::ComPtr<ID3D12Resource> textureResource = CreateTextureResource(device, metadata);
    _textureResources.push_back(textureResource);
    UploadTextureData(textureResource, mipImage);

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = metadata.format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

    numUploadedTexture++;
    D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = GetCPUDescriptorHandle(_srvDescriptorHeap, _kDescriptorSizeSRV, numUploadedTexture);
    D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU = GetGPUDescriptorHandle(_srvDescriptorHeap, _kDescriptorSizeSRV, numUploadedTexture);
    textureSrvHandleCPUs.push_back(textureSrvHandleCPU);
    textureSrvHandleGPUs.push_back(textureSrvHandleGPU);
    device->CreateShaderResourceView(textureResource.Get(), &srvDesc, textureSrvHandleCPU);
    return;
}

Microsoft::WRL::ComPtr<ID3D12Resource> DX12Helper::CreateVertexResource(const Microsoft::WRL::ComPtr<ID3D12Device>& _device, unsigned int _countVertex)
{
    // 頂点リソースを作る
    Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = CreateBufferResource(_device, sizeof(VertexData) * _countVertex);
    return vertexResource;
}

void DX12Helper::ChangeStateResource(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& _commandList, const Microsoft::WRL::ComPtr<ID3D12Resource>& _resource, D3D12_RESOURCE_STATES _before, D3D12_RESOURCE_STATES _after)
{
    D3D12_RESOURCE_BARRIER barrier{};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = _resource.Get();
    barrier.Transition.StateBefore = _before;
    barrier.Transition.StateAfter = _after;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    _commandList->ResourceBarrier(1, &barrier);
}

void DX12Helper::CommandListCommonSetting(const DirectX12* _pDx12, ID3D12GraphicsCommandList* _commandList, const D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandle)
{
    /// 必要なデータを取得
    const SRVManager* pSrvManager = SRVManager::GetInstance();
    ID3D12DescriptorHeap* ppHeaps[] = { pSrvManager->GetDescriptorHeap() };

    auto dsvHandle      = _pDx12->GetDSVDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
    auto viewport       = _pDx12->GetViewport();
    auto scissorRect    = _pDx12->GetScissorRect();

    /// コマンドリストの設定
    _commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
    _commandList->OMSetRenderTargets(1, rtvHandle, FALSE, &dsvHandle);
    _commandList->RSSetViewports(1, &viewport);
    _commandList->RSSetScissorRects(1, &scissorRect);

    return;
}

Microsoft::WRL::ComPtr<ID3D12Resource> DX12Helper::CreateRenderTextureResource(const ComPtr<ID3D12Device>& _device, int32_t _width, int32_t _height, DXGI_FORMAT _format, const Vector4& _clearColor)
{
    Microsoft::WRL::ComPtr<ID3D12Resource> result = nullptr;
    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
    D3D12_RESOURCE_DESC resourceDesc = {};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    resourceDesc.Width = _width;
    resourceDesc.Height = _height;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    resourceDesc.Format = _format;

    D3D12_CLEAR_VALUE clearValue = {};
    clearValue.Format = _format;
    clearValue.Color[0] = _clearColor.x;
    clearValue.Color[1] = _clearColor.y;
    clearValue.Color[2] = _clearColor.z;
    clearValue.Color[3] = _clearColor.w;

    HRESULT hr = _device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        &clearValue,
        IID_PPV_ARGS(&result));

    if (FAILED(hr))
    {
        Logger::GetInstance()->LogError(__FILE__, __FUNCTION__, "CreateCommittedResource failed");
        assert(false && "Create committed resource failed");
    }

    return result;
}

D3D12_CPU_DESCRIPTOR_HANDLE DX12Helper::GetCPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& _descriptorHeap, uint32_t _descriptorSize, uint32_t _index)
{
    D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = _descriptorHeap->GetCPUDescriptorHandleForHeapStart();
    handleCPU.ptr += (_descriptorSize * _index); // ポインタをヒープの始めからインデックス分インクリメント
    return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE DX12Helper::GetGPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& _descriptorHeap, uint32_t _descriptorSize, uint32_t _index)
{
    D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = _descriptorHeap->GetGPUDescriptorHandleForHeapStart();
    handleGPU.ptr += (_descriptorSize * _index);
    return handleGPU;
}