#include "Model.h"

#include <Core/DirectX12/Helper/DX12Helper.h>
#include <Features/Model/Helper/ModelHelper.h>
#include <Features/Object3d/Object3dSystem.h>
#include <Core/DirectX12/TextureManager.h>
#include <Features/Model/ModelManager.h>
#include <DebugTools/DebugManager/DebugManager.h>
#include <filesystem>

void Model::Initialize(const std::string& _filePath)
{
    /// 必要なインスタンスを取得
    pDx12_ = DirectX12::GetInstance();
    device_ = pDx12_->GetDevice();

    filePath_ = _filePath;

    /// モデルデータを読み込む
    directoryPath_ = ModelManager::GetInstance()->GetDirectoryPath(filePath_);
    modelData_ = ModelHelper::LoadObjFile(directoryPath_, filePath_);
    DebugManager::GetInstance()->PushLog("[ロード完了] path: " + filePath_ + "\n");

    /// 頂点リソースを作成
    CreateVertexResource();

    /// テクスチャを読み込む
    LoadModelTexture();

}

void Model::Update()
{
}

void Model::Draw()
{
    ID3D12GraphicsCommandList* commandList = pDx12_->GetCommandList();

    // 頂点バッファを設定
    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
    // SRVの設定
    commandList->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_);
    // 描画！（DrawCall/ドローコール）。頂点
    commandList->DrawInstanced(static_cast<uint32_t>(modelData_.vertices.size()), 1, 0, 0);
}

Model::~Model()
{
    OutputDebugStringA("Model Destructor\n");
}

void Model::CreateVertexResource()
{
    /// 頂点リソースを作成
    vertexResource_ = DX12Helper::CreateBufferResource(device_, sizeof(VertexData) * modelData_.vertices.size());
    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

    /// 頂点データを初期化
    std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

    /// 頂点バッファービューを初期化
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = static_cast<uint32_t>(sizeof(VertexData) * modelData_.vertices.size());
    vertexBufferView_.StrideInBytes = sizeof(VertexData);
}


void Model::LoadModelTexture()
{
    std::string filePath = modelData_.materialData.textureFilePath;

    if (!std::filesystem::exists(modelData_.materialData.textureFilePath))
    {
        DebugManager::GetInstance()->PushLog("[Warning] The model's texture could not be loaded so white1x1.png is loaded instead.\n\t path: " + modelData_.materialData.textureFilePath + "\n");
        filePath = "white1x1.png";
    }

    TextureManager* textureManager = TextureManager::GetInstance();
    textureManager->LoadTexture(filePath);
    textureSrvHandleGPU_ = TextureManager::GetInstance()->GetSrvHandleGPU(filePath);
}