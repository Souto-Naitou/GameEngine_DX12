#include "Object3d.h"

#include <Matrix4x4.h>
#include <Common/structs.h>
#include <Core/Win32/WinSystem.h>
#include <Core/DirectX12/DirectX12.h>
#include <Core/DirectX12/Helper/DX12Helper.h>
#include <Core/DirectX12/TextureManager.h>
#include <Features/Model/ModelManager.h>
#include <Utility/Debug/dbgutl.h>


#if defined (DEBUG_ENGINE) && (_DEBUG)
#include <DebugTools/DebugManager/DebugManager.h>
#include <imgui.h>
#endif // DEBUG_ENGINE && _DEBUG


void Object3d::Initialize(bool _enableDebugWindow)
{
    /// 必要なインスタンスを取得
    pSystem_ = Object3dSystem::GetInstance();
    pDx12_ = pSystem_->GetDirectX12();
    device_ = pDx12_->GetDevice();
    ppSystemGameEye_ = pSystem_->GetGlobalEye();

    isEnableDebugWindow_ = _enableDebugWindow;

#if defined (DEBUG_ENGINE) && (_DEBUG)
    if (isEnableDebugWindow_)
    {
        pDebugManager_ = DebugManager::GetInstance();
        name_ = utl::debug::generate_name_default(this);
        pDebugManager_->SetComponent("Object3d", name_, std::bind(&Object3d::DebugWindow, this));
    }
#endif // DEBUG_ENGINE && _DEBUG

    transform_.scale = Vector3(1.0f, 1.0f, 1.0f);
    transform_.rotate = Vector3(0.0f, 0.0f, 0.0f);
    transform_.translate = Vector3(0.0f, 0.0f, 0.0f);


    /// 座標変換行列リソースを作成
    CreateTransformationMatrixResource();

    /// 平行光源リソースを作成
    CreateDirectionalLightResource();

    /// テクスチャのタイリングリソースを作成
    CreateTilingResource();

    /// カメラのワールド座標リソースを作成
    CreateCameraForGPUResource();

    /// ライティングリソースを作成
    CreateLightingResource();

    /// ポイントライトリソースを作成
    CreatePointLightResource();

    /// マテリアルリソースを作成
    CreateMaterialResource();
}

void Object3d::Update()
{
    if (!isUpdate_) return;

    rotateMatrix_ = Matrix4x4::RotateXMatrix(transform_.rotate.x) * (Matrix4x4::RotateYMatrix(transform_.rotate.y) * Matrix4x4::RotateZMatrix(transform_.rotate.z));
    Matrix4x4 wMatrix = Matrix4x4::AffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
    Matrix4x4 wvpMatrix = {};

    const auto& localMatrix = pModel_->GetModelData()->rootNode.localMatrix;

    /// カメラの行列計算
    auto vpMatrix = Matrix4x4::Identity();
    if (pGameEye_)
    {
        vpMatrix = pGameEye_->GetViewProjectionMatrix();
    }
    else if (ppSystemGameEye_ && *ppSystemGameEye_)
    {
        vpMatrix = (*ppSystemGameEye_)->GetViewProjectionMatrix();
    }

    wvpMatrix = wMatrix * vpMatrix;

    /// 座標変換行列データを更新
    transformationMatrixData_->wvp = wvpMatrix;
    transformationMatrixData_->world = wMatrix;


    /// 平行光源の方向を正規化
    if (directionalLight_)
        directionalLight_->direction = directionalLight_->direction.Normalize();


    /// 平行光源データを更新
    if (directionalLight_)
    {
        directionalLightData_->color = directionalLight_->color;
        directionalLightData_->direction = directionalLight_->direction;
        directionalLightData_->intensity = directionalLight_->intensity;
    }


    /// ポイントライトデータを更新
    if (pointLight_)
    {
        pointLightData_->enablePointLight = pointLight_->enablePointLight;
        pointLightData_->color = pointLight_->color;
        pointLightData_->position = pointLight_->position;
        pointLightData_->intensity = pointLight_->intensity;
    }


    /// カメラのワールド座標を更新
    if (pGameEye_)
    {
        cameraForGPU_->worldPosition = pGameEye_->GetTransform().translate;
    }
    else if (ppSystemGameEye_ && *ppSystemGameEye_)
    {
        cameraForGPU_->worldPosition = (*ppSystemGameEye_)->GetTransform().translate;
    }


}

void Object3d::Draw()
{
    if (!isDraw_) return;

    Object3dSystem::CommandListData data;
    data.cbuffers[0] = materialResource_.Get();
    data.cbuffers[1] = transformationMatrixResource_.Get();
    data.cbuffers[3] = directionalLightResource_.Get();
    data.cbuffers[4] = tilingResource_.Get();
    data.cbuffers[5] = cameraForGPUResource_.Get();
    data.cbuffers[6] = lightingResource_.Get();
    data.cbuffers[7] = pointLightResource_.Get();
    data.model = pModel_;

    pSystem_->AddCommandListData(data);
}

void Object3d::Finalize()
{
#if defined (DEBUG_ENGINE) && (_DEBUG)
    if (isEnableDebugWindow_)
    {
        pDebugManager_->DeleteComponent("Object3d", name_.c_str());
    }
#endif // DEBUG_ENGINE && _DEBUG
}


void Object3d::CreateTransformationMatrixResource()
{
    /// 座標変換行列リソースを作成
    transformationMatrixResource_ = DX12Helper::CreateBufferResource(device_, sizeof(TransformationMatrix));
    transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
    /// 座標変換行列データを初期化
    transformationMatrixData_->wvp = Matrix4x4::Identity();
    transformationMatrixData_->world = Matrix4x4::Identity();
}

void Object3d::CreateDirectionalLightResource()
{
    /// 平行光源リソースを作成
    directionalLightResource_ = DX12Helper::CreateBufferResource(device_, sizeof(DirectionalLight));
    directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
    /// 平行光源データを初期化
    directionalLightData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    directionalLightData_->direction = Vector3(0.0f, -1.0f, 0.0f);
    directionalLightData_->intensity = 1.0f;
}

void Object3d::CreateTilingResource()
{
    /// テクスチャのタイリングリソースを作成
    tilingResource_ = DX12Helper::CreateBufferResource(device_, sizeof(TilingData));
    tilingResource_->Map(0, nullptr, reinterpret_cast<void**>(&tilingData_));
    /// タイリングデータを初期化
    tilingData_->tilingMultiply = Vector2(1.0f, 1.0f);
}

void Object3d::CreateCameraForGPUResource()
{
    cameraForGPUResource_ = DX12Helper::CreateBufferResource(device_, sizeof(CameraForGPU));
    cameraForGPUResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraForGPU_));
    cameraForGPU_->worldPosition = Vector3();
}

void Object3d::CreateLightingResource()
{
    lightingResource_ = DX12Helper::CreateBufferResource(device_, sizeof(Lighting));
    lightingResource_->Map(0, nullptr, reinterpret_cast<void**>(&lightingData_));
    lightingData_->enableLighting = 1;
    lightingData_->lightingType = LightingType::HarfLambert;
}

void Object3d::CreatePointLightResource()
{
    pointLightResource_ = DX12Helper::CreateBufferResource(device_, sizeof(PointLight));
    pointLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));
    pointLightData_->enablePointLight = 0;
    pointLightData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    pointLightData_->position = Vector3(0.0f, 0.0f, 0.0f);
    pointLightData_->intensity = 1.0f;
}

void Object3d::CreateMaterialResource()
{
    /// マテリアルリソースを作成
    materialResource_ = DX12Helper::CreateBufferResource(device_, sizeof(Material));
    materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
    /// マテリアルデータを初期化
    materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    materialData_->uvTransform = Matrix4x4::Identity();
    materialData_->shininess = 1.0f;
}

void Object3d::CreateUAV()
{
    auto srvManager = SRVManager::GetInstance();
    uavIndex_ = srvManager->Allocate();

    SRVManager::GetInstance()->CreateUAV4Buffer(uavIndex_, outputResource_, DXGI_FORMAT_UNKNOWN, pModel_->GetVertexCount(), sizeof(VertexData));
}

#ifdef DEBUG_ENGINE
void Object3d::DebugWindow()
{
#ifdef _DEBUG

    ImGui::Checkbox("Update", &isUpdate_);
    ImGui::SameLine();
    ImGui::Checkbox("Draw2D", &isDraw_);

    /// 変形
    ImGui::SeparatorText("Transform");
    {
        ImGui::PushID("TRANSFORM");
        ImGui::DragFloat3("Scale", &transform_.scale.x, 0.01f);
        ImGui::DragFloat3("Rotate", &transform_.rotate.x, 0.01f);
        ImGui::DragFloat3("Translate", &transform_.translate.x, 0.01f);
        ImGui::PopID();
    }


    /// マテリアル
    ImGui::SeparatorText("Material");
    {
        ImGui::PushID("MATERIAL");
        ImGui::ColorEdit4("Color", &materialData_->color.x);
        ImGui::DragFloat2("UV Offset", &materialData_->uvTransform.m[3][0], 0.01f);
        ImGui::DragFloat2("UV Tiling", &materialData_->uvTransform.m[0][0], 0.01f);
        ImGui::PopID();
    }


    /// 平行光源
    ImGui::SeparatorText("Directional Light");
    {
        ImGui::PushID("DIRECTIONAL_LIGHT");
        if (ImGui::Checkbox("Enable Lighting", &isEnableLighting_))
        {
            lightingData_->enableLighting = isEnableLighting_;
        }

        ImGui::SameLine();

        const char* items[] = { "Lambertian Reflectance", "Harf Lambert" };
        ImGui::Combo("##Lighting Type", reinterpret_cast<int*>(&lightingData_->lightingType), items, 2);

        if (directionalLight_)
        {
            ImGui::DragFloat("Shininess", &materialData_->shininess, 0.01f);
            ImGui::ColorEdit4("Color", &directionalLight_->color.x);
            ImGui::DragFloat3("Direction", &directionalLight_->direction.x, 0.01f);
            ImGui::DragFloat("Intensity", &directionalLight_->intensity, 0.01f);
        }
        ImGui::PopID();
    }


    /// ポイントライト
    ImGui::SeparatorText("Point Light");
    {
        ImGui::PushID("POINT_LIGHT");
        if (pointLight_)
        {
            bool enablePointLight = pointLight_->enablePointLight;
            if (ImGui::Checkbox("Enable PointLight", &enablePointLight))
            {
                pointLight_->enablePointLight = enablePointLight;
            }
            ImGui::ColorEdit4("Color", &pointLight_->color.x);
            ImGui::DragFloat3("Position", &pointLight_->position.x, 0.01f);
            ImGui::DragFloat("Intensity", &pointLight_->intensity, 0.01f);
        }
        ImGui::PopID();
    }


    /// タイリング
    ImGui::SeparatorText("Tiling");
    {
        ImGui::PushID("TILING");
        ImGui::DragFloat2("Tiling Multiply", &tilingData_->tilingMultiply.x, 0.01f);
        ImGui::PopID();
    }


#endif // _DEBUG
}
#endif // DEBUG_ENGINE
