#include "Viewport.h"

#include <DebugTools/Logger/Logger.h>
#include <Core/DirectX12/Helper/DX12Helper.h>

#include <d3dcompiler.h>
#include <imgui.h>

#include <cassert>

#pragma comment(lib, "d3dcompiler.lib")

void Viewport::Initialize()
{
    /// インスタンスの取得
    pDx12_ = DirectX12::GetInstance();
    pSRVManager_ = SRVManager::GetInstance();
    device_ = pDx12_->GetDevice();
    dxcUtils_ = pDx12_->GetDxcUtils();
    dxcCompiler_ = pDx12_->GetDxcCompiler();
    includeHandler_ = pDx12_->GetIncludeHandler();
    commandList_ = pDx12_->GetCommandList();

    viewport_ = pDx12_->GetViewport();

    inputTexture_ = pDx12_->GetGameScreenResource();
    outputTexture_ = pDx12_->GetGameScreenComputed();

    // ルートシグネチャの生成
    CreateRootSignature();

    // パイプラインステートオブジェクトの生成
    CreatePSO();

    // SRVの生成
    CreateSRV();

    // UAVの生成
    CreateUAV();

    pDx12_->SetGameWndSRVIndex(outputSRVIndex_);
}

void Viewport::CreateRootSignature()
{
    /// RootSignature作成
    D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
    descriptionRootSignature.Flags =
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    /// RootParameter作成 (SRV & UAV)
    D3D12_DESCRIPTOR_RANGE srvRange = {};
    srvRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    srvRange.NumDescriptors = 1;
    srvRange.BaseShaderRegister = 0;
    srvRange.RegisterSpace = 0;
    srvRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    D3D12_DESCRIPTOR_RANGE uavRange = {};
    uavRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
    uavRange.NumDescriptors = 1;
    uavRange.BaseShaderRegister = 0;
    uavRange.RegisterSpace = 0;
    uavRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    /// 作成
    D3D12_ROOT_PARAMETER rootParameters[2] = {};
    rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;       // SRVを使う
    rootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
    rootParameters[0].DescriptorTable.pDescriptorRanges = &srvRange;
    rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;                   // どのシェーダーでも使う

    rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;       // UAVを使う
    rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
    rootParameters[1].DescriptorTable.pDescriptorRanges = &uavRange;
    rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;                   // どのシェーダーでも使う


    descriptionRootSignature.pParameters = rootParameters;                              // ルートパラメータ配列へのポインタ
    descriptionRootSignature.NumParameters = _countof(rootParameters);                  // 配列の長さ
    descriptionRootSignature.pStaticSamplers = nullptr;
    descriptionRootSignature.NumStaticSamplers = 0;


    // シリアライズしてバイナリにする
    Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
    HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature,
        D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
    if (FAILED(hr))
    {
        Logger::GetInstance()->LogError(
            "Viewport",
            "CreateRootSignature",
            reinterpret_cast<char*>(errorBlob->GetBufferPointer())
        );
        assert(false);
    }
    // バイナリをもとに生成
    hr = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
        signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
    assert(SUCCEEDED(hr));

    return;
}

void Viewport::CreatePSO()
{
    D3D12_COMPUTE_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.pRootSignature = rootSignature_.Get();

    hr_ = D3DCompileFromFile(
        kComputeShaderPath_,
        nullptr,
        nullptr,
        "CSMain",
        "cs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &csBlob_,
        &errorBlob_
    );

    psoDesc.CS = { csBlob_->GetBufferPointer(), csBlob_->GetBufferSize() };

    device_->CreateComputePipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState_));
    
}

void Viewport::CreateSRV()
{
    inputSRVIndex_ = pSRVManager_->Allocate();
    pSRVManager_->CreateForTexture2D(inputSRVIndex_, inputTexture_, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 1);

    outputSRVIndex_ = pSRVManager_->Allocate();
    pSRVManager_->CreateForTexture2D(outputSRVIndex_, outputTexture_, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 1);
}

void Viewport::CreateUAV()
{
    outputUAVIndex_ = pSRVManager_->Allocate();
    pSRVManager_->CreateForUAV(outputUAVIndex_, outputTexture_, DXGI_FORMAT_R8G8B8A8_UNORM);
}

void Viewport::Compute()
{
    #ifdef _DEBUG

    /// ステートの変更
    DX12Helper::ChangeStateResource(
        commandList_, 
        outputTexture_, 
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        D3D12_RESOURCE_STATE_UNORDERED_ACCESS
    );

    DX12Helper::ChangeStateResource(
        commandList_,
        inputTexture_,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE
    );

    commandList_->SetPipelineState(pipelineState_.Get());
    commandList_->SetComputeRootSignature(rootSignature_.Get());


    D3D12_GPU_DESCRIPTOR_HANDLE gpuSrvHandle = pSRVManager_->GetGPUDescriptorHandle(inputSRVIndex_);
    D3D12_GPU_DESCRIPTOR_HANDLE gpuUavHandle = pSRVManager_->GetGPUDescriptorHandle(outputUAVIndex_);


    // SRV & UAV をセット
    commandList_->SetComputeRootDescriptorTable(0, gpuSrvHandle);
    commandList_->SetComputeRootDescriptorTable(1, gpuUavHandle);


    // 画像サイズに応じて Dispatch
    commandList_->Dispatch(
        static_cast<UINT>(viewport_.Width + 15) / 16,
        static_cast<UINT>(viewport_.Height + 15) / 16,
        1
    );

    DX12Helper::ChangeStateResource(
        commandList_,
        inputTexture_,
        D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
    );

    /// ステートの変更
    DX12Helper::ChangeStateResource(
        commandList_,
        outputTexture_,
        D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
    );

    #endif // _DEBUG
}

void Viewport::DrawWindow()
{
    #ifdef _DEBUG

    auto gpuHnd = SRVManager::GetInstance()->GetGPUDescriptorHandle(outputSRVIndex_);
    auto vp = pDx12_->GetViewport();

    uint32_t width = static_cast<uint32_t>(vp.Width);
    uint32_t height = static_cast<uint32_t>(vp.Height);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));

    if(ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
    {
        //auto pos = ImGui::GetWindowPos();
        //windowPos_ = { pos.x, pos.y };

        //auto wndSize = ImGui::GetWindowSize();
        //vpSize_ = { wndSize.x, wndSize.y };

        auto cliSize = ImGui::GetContentRegionAvail();

        float aspect = static_cast<float>(width) / static_cast<float>(height);

        ImVec2 imageSizeAdjusted = cliSize;
        imageSizeAdjusted.x = imageSizeAdjusted.y * aspect;

        ImGui::Image((ImTextureID)gpuHnd.ptr, imageSizeAdjusted);

        auto itempos = ImGui::GetItemRectMin();
        vpPos_ = { itempos.x, itempos.y };
        auto itemsize = ImGui::GetItemRectSize();
        vpSize_ = { itemsize.x, itemsize.y };
    }
    ImGui::End();

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    #endif // _DEBUG
}
