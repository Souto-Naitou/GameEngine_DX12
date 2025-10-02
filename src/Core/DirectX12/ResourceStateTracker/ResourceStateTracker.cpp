#include "./ResourceStateTracker.h"

void ResourceStateTracker::Reset()
{
    resource.Reset();
    state = D3D12_RESOURCE_STATE_COMMON;
}

void ResourceStateTracker::ChangeState(ID3D12GraphicsCommandList* _commandList, D3D12_RESOURCE_STATES _newState)
{
    if (this->state == _newState) return;

    D3D12_RESOURCE_BARRIER barrier{};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = this->resource.Get();
    barrier.Transition.StateBefore = this->state;
    barrier.Transition.StateAfter = _newState;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    _commandList->ResourceBarrier(1, &barrier);
    this->state = _newState;
}
