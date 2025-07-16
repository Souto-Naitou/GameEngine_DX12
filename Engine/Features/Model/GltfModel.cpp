#include "./GltfModel.h"
#include <DebugTools/DebugManager/DebugManager.h>
#include <Core/DirectX12/TextureManager.h>
#include <Features/Model/Helper/AnimationHelper.h>
#include <stdexcept>

GltfModel::~GltfModel()
{
    if (is_called_finalize_ == false)
    {
        assert(false && "GltfModel::Finalize() must be called before destruction.");
    }
}

void GltfModel::Initialize()
{
    isOverwroteTexture_ = false;
    timer_.Reset();
    timer_.Start();
    srvManager_ = SRVManager::GetInstance();
}

void GltfModel::Finalize()
{
    is_called_finalize_ = true;
}

void GltfModel::Update()
{
    // 遅延クローン処理
    if (pCloneSrc_ != nullptr)
    {
        // クローン元のモデルが読み込み完了している場合
        if (pCloneSrc_->IsEndLoading())
        {
            // クローン元からデータをコピー
            this->_CopyFrom(pCloneSrc_);
            pCloneSrc_ = nullptr; // クローン元をリセット
        }
    }

    //this->_UpdateLocalMatrixByAnimation();

    this->_ApplyAnimationToSkeleton();

    this->_UpdateSkeleton();

    this->_UpdateSkinCluster();
}

void GltfModel::Draw(ID3D12GraphicsCommandList* _cl)
{
    if (isReadyDraw_ == false) return;

    // 頂点バッファビューを設定
    _cl->IASetVertexBuffers(0, 1, &vertexBufferView_);
    // インデックスバッファを設定
    _cl->IASetIndexBuffer(&indexBufferView_);
    // SRVの設定
    _cl->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_);
    // 描画！（DrawCall/ドローコール）
    _cl->DrawIndexedInstanced(
        static_cast<UINT>(modelData_.indices.size()), // インデックス数
        1, // インスタンス数
        0, // インデックスバッファの開始オフセット
        0, // 頂点バッファの開始オフセット
        0  // グループID（インスタンス化描画用）
    );
}

void GltfModel::ChangeTexture(D3D12_GPU_DESCRIPTOR_HANDLE _texSrvHnd)
{
    textureSrvHandleGPU_ = _texSrvHnd;
    isOverwroteTexture_ = true;
}

void GltfModel::CreateGPUResource()
{
    /// 頂点リソースを作成
    this->_CreateVertexResource();

    /// インデックスリソースを作成
    this->_CreateIndexResource();

    /// テクスチャを読み込む
    this->_LoadModelTexture();

    /// スキニング結果を格納するリソースを作成
    this->_CreateSkinnedResource();

    /// UAVの生成
    this->_CreateUAV();

    // フラグを立てる
    isReadyDraw_ = true;
}

bool GltfModel::IsEndLoading() const
{
    return isReadyDraw_;
}

void GltfModel::Clone(IModel* _src)
{
    if (_src == nullptr) return;

    isOverwroteTexture_ = false;

    // クローン元がGltfModelでない場合はエラー
    GltfModel* pSrc = dynamic_cast<GltfModel*>(_src);
    if (!pSrc)
    {
        throw std::runtime_error("GltfModel::Clone: Source model is not a GltfModel.");
        return;
    }

    if (pSrc->IsEndLoading() == false)
    {
        // Updateで読み込みが終わるまで待つ
        // データのコピーはUpdateで行われる
        pCloneSrc_ = pSrc;
        return;
    }

    this->_CopyFrom(pSrc);
}

void GltfModel::DispatchSkinning()
{
    auto cl = pDx12_->GetCommandList();

    // リソースの状態を更新
    DX12Helper::ChangeStateResource(
        cl,
        resourceSkinned_,
        D3D12_RESOURCE_STATE_UNORDERED_ACCESS
    );

    // 準備
    ID3D12DescriptorHeap* ppHeaps[] = { srvManager_->GetDescriptorHeap() };
    cl->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
    cl->SetComputeRootDescriptorTable(0, skinCluster_.srvHandlePalette.second);
    cl->SetComputeRootDescriptorTable(1, srvHandleGpuInputVertex_);
    cl->SetComputeRootDescriptorTable(2, skinCluster_.srvHandleInfluence.second);
    cl->SetComputeRootDescriptorTable(3, srvHandleGpuSkinned_);
    cl->SetComputeRootConstantBufferView(4, skinCluster_.resourceSkinningInformation->GetGPUVirtualAddress());

    cl->Dispatch(
        static_cast<uint32_t>(modelData_.vertices.size() + 1023) / 1024, // グループ数X
        1, // グループ数Y
        1  // グループ数Z
    );

    // リソースの状態を更新
    DX12Helper::ChangeStateResource(
        cl,
        resourceSkinned_,
        D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
    );
}

D3D12_VERTEX_BUFFER_VIEW GltfModel::GetVertexBufferView() const
{
    return vertexBufferView_;
}

size_t GltfModel::GetVertexCount() const
{
    return modelData_.vertices.size();
}

D3D12_GPU_DESCRIPTOR_HANDLE GltfModel::GetTextureSrvHandle() const
{
    return textureSrvHandleGPU_;
}

ModelData* GltfModel::GetModelData()
{
    return &modelData_;
}

Animation* GltfModel::GetAnimationData()
{
    return &animationData_;
}

Skeleton* GltfModel::GetSkeleton()
{
    return &skeleton_;
}

SkinCluster* GltfModel::GetSkinCluster()
{
    return &skinCluster_;
}

void GltfModel::_CreateVertexResource()
{
    /// 頂点リソースを作成
    vertexResource_ = DX12Helper::CreateBufferResource(pDx12_->GetDevice(), sizeof(VertexData) * modelData_.vertices.size());
    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

    /// 頂点データを初期化
    std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

    /// SRVの生成
    srvIndexInputVertex_ = srvManager_->Allocate();
    srvManager_->CreateForStructuredBuffer(srvIndexInputVertex_, vertexResource_.Get(), static_cast<uint32_t>(modelData_.vertices.size()), sizeof(VertexData));
    srvHandleGpuInputVertex_ = srvManager_->GetGPUDescriptorHandle(srvIndexInputVertex_);
}

void GltfModel::_CreateIndexResource()
{
    /// インデックスリソースを作成
    indexResource_ = DX12Helper::CreateBufferResource(pDx12_->GetDevice(), sizeof(uint32_t) * modelData_.indices.size());
    indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

    /// インデックスデータを初期化
    std::memcpy(indexData_, modelData_.indices.data(), sizeof(uint32_t) * modelData_.indices.size());

    /// インデックスバッファービューを初期化
    indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
    indexBufferView_.SizeInBytes = static_cast<uint32_t>(sizeof(uint32_t) * modelData_.indices.size());
    indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
}

void GltfModel::_CreateSkinnedResource()
{
    resourceSkinned_.resource = DX12Helper::CreateBufferResource(
        pDx12_->GetDevice(),
        sizeof(VertexData) * modelData_.vertices.size(),
        D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS
    );

    /// 頂点バッファービューを初期化
    vertexBufferView_.BufferLocation = resourceSkinned_.resource->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = static_cast<uint32_t>(sizeof(VertexData) * modelData_.vertices.size());
    vertexBufferView_.StrideInBytes = sizeof(VertexData);
}


void GltfModel::_LoadModelTexture()
{
    if (isOverwroteTexture_)
    {
        // テクスチャを上書きした場合は、マテリアルファイル記述のテクスチャを使用しない
        return;
    }

    std::string filePath = modelData_.material.textureFilePath;

    if (!std::filesystem::exists(modelData_.material.textureFilePath))
    {
        DebugManager::GetInstance()->PushLog("[Warning] The model's texture could not be loaded so white1x1.png is loaded instead : " + modelData_.material.textureFilePath + "\n");
        Logger::GetInstance()->LogWarning(
            "Model",
            "LoadModelTexture",
            "Could'nt load so white1x1.png is loaded instead. path: " + modelData_.material.textureFilePath
        );

        filePath = "white1x1.png";
    }

    TextureManager* textureManager = TextureManager::GetInstance();
    textureManager->LoadTexture(filePath);
    textureSrvHandleGPU_ = textureManager->GetSrvHandleGPU(filePath);

}

void GltfModel::_CopyFrom(GltfModel* _pCopySrc)
{
    this->pDx12_ = _pCopySrc->pDx12_;
    // モデルデータをコピー
    this->modelData_ = _pCopySrc->modelData_;
    this->animationData_ = _pCopySrc->animationData_;
    // テクスチャのSRVハンドルをコピー
    // NOTE: クローン元のテクスチャを上書きしていない場合に限る
    if (!isOverwroteTexture_)
    {
        this->textureSrvHandleGPU_ = _pCopySrc->textureSrvHandleGPU_;
    }
    this->skeleton_ = _pCopySrc->skeleton_;
    this->skinCluster_ = _pCopySrc->skinCluster_;
    // リソースを作成
    this->_CreateVertexResource();
    this->_CreateIndexResource();
    this->_CreateSkinnedResource();
    this->_CreateUAV();

    isReadyDraw_ = true;
}

void GltfModel::_UpdateLocalMatrixByAnimation()
{
    if (timer_.GetNow<float>() >= animationData_.duration)
    {
        timer_.Reset();
        timer_.Start();
    }

    // RootAnimationの取得
    NodeAnimation& rootNodeAnim = animationData_.nodeAnimations[modelData_.rootNode.name];
    Vector3 scale = Helper::Animation::CalculateValue(rootNodeAnim.scale, timer_.GetNow<float>());
    Quaternion rotation = Helper::Animation::CalculateValue(rootNodeAnim.rotation, timer_.GetNow<float>());
    Vector3 translation = Helper::Animation::CalculateValue(rootNodeAnim.translate, timer_.GetNow<float>());
    Matrix4x4 localMatrix = Matrix4x4::AffineMatrix(scale, rotation, translation);

    modelData_.rootNode.localMatrix = localMatrix;
}

void GltfModel::_UpdateSkeleton()
{
    for (Joint& joint : skeleton_.GetSkeletonData().joints)
    {
        JointData& jointData = joint.GetJointData();

        jointData.localMatrix = Matrix4x4::AffineMatrix(
            jointData.transform.scale,
            jointData.transform.rotate,
            jointData.transform.translate
        );

        if (jointData.parentIndex)
        {
            // 親ジョイントが存在するなら親の行列を掛ける
            const Matrix4x4& parentSkeletonSpaceMatrix = skeleton_.GetSkeletonData().joints[*jointData.parentIndex].GetJointData().skeletonSpaceMatrix;
            jointData.skeletonSpaceMatrix = jointData.localMatrix * parentSkeletonSpaceMatrix;
        }
        else
        {
            // 親ジョイントがいないのでlocalMatrixとskeletonSpaceMatrixは同じ
            jointData.skeletonSpaceMatrix = jointData.localMatrix;
        }
    }

    skeleton_.Update();
}

void GltfModel::_UpdateSkinCluster()
{
    auto& skeletonData = skeleton_.GetSkeletonData();
    for (size_t jointIndex = 0; jointIndex < skeletonData.joints.size(); ++jointIndex)
    {
        if (jointIndex >= skinCluster_.inverseBindPoseMatrices.size())
        {
            // Jointの数がSkinClusterの逆バインドポーズマトリックスの数を超えた場合はエラー
            throw std::runtime_error("Joint index exceeds the size of inverseBindPoseMatrices in SkinCluster.");
        }
        skinCluster_.mappedPalette[jointIndex].skeletonSpaceMatrix =
            skinCluster_.inverseBindPoseMatrices[jointIndex] * skeletonData.joints[jointIndex].GetJointData().skeletonSpaceMatrix;
        skinCluster_.mappedPalette[jointIndex].skeletonSpaceInverseTransposeMatrix =
            skinCluster_.mappedPalette[jointIndex].skeletonSpaceMatrix.Inverse().Transpose();
    }
}

void GltfModel::_ApplyAnimationToSkeleton()
{
    if (timer_.GetNow<float>() >= animationData_.duration)
    {
        timer_.Reset();
        timer_.Start();
    }

    SkeletonData& skeletonData = skeleton_.GetSkeletonData();
    for (Joint& joint : skeletonData.joints)
    {
        JointData& jointData = joint.GetJointData();
        if (auto it = animationData_.nodeAnimations.find(jointData.name); it != animationData_.nodeAnimations.end())
        {
            const NodeAnimation& rootNodeAnimation = (*it).second;
            // アニメーションの適用
            jointData.transform.translate = Helper::Animation::CalculateValue(rootNodeAnimation.translate, timer_.GetNow<float>());
            jointData.transform.rotate = Helper::Animation::CalculateValue(rootNodeAnimation.rotation, timer_.GetNow<float>());
            jointData.transform.scale = Helper::Animation::CalculateValue(rootNodeAnimation.scale, timer_.GetNow<float>());
        }
    }
}

void GltfModel::_CreateUAV()
{
    srvIndexSkinned_ = srvManager_->Allocate();
    srvHandleGpuSkinned_ = srvManager_->GetGPUDescriptorHandle(srvIndexSkinned_);

    srvManager_->CreateUAV4Buffer(
        srvIndexSkinned_,
        resourceSkinned_.resource.Get(),
        DXGI_FORMAT_UNKNOWN,
        static_cast<uint32_t>(modelData_.vertices.size()),
        sizeof(VertexData)
    );
}
