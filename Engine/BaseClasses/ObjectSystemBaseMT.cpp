#include "./ObjectSystemBaseMT.h"

void ObjectSystemBaseMT::Initialize()
{
    CreateCommandList();
}

void ObjectSystemBaseMT::PostDraw()
{
    commandAllocator_->Reset();
    commandList_->Reset(commandAllocator_.Get(), nullptr);
}

ID3D12GraphicsCommandList* ObjectSystemBaseMT::GetCommandList()
{
    return commandList_.Get();
}

GameEye** ObjectSystemBaseMT::GetGlobalEye()
{
    return &pGlobalEye_;
}

void ObjectSystemBaseMT::SetGlobalEye(GameEye* _pGameEye)
{
    pGlobalEye_ = _pGameEye;
}

void ObjectSystemBaseMT::SetRTVHandle(D3D12_CPU_DESCRIPTOR_HANDLE* _handle)
{
    rtvHandle_ = _handle;
}

void ObjectSystemBaseMT::CreateCommandList()
{
    auto device = pDx12_->GetDevice();
    device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocator_.GetAddressOf()));
    device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(commandList_.GetAddressOf()));
}
