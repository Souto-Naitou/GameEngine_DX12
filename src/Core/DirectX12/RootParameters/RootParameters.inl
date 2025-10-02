#pragma once

#include <cstddef>

template <int _Size>
typename RootParameters<_Size>::_ref
RootParameters<_Size>::SetParameter(int _index, const std::string& _slot, D3D12_SHADER_VISIBILITY _visibility)
{
    if (_slot.empty())
        throw std::invalid_argument("Slot name cannot be empty.");

    if (_index >= _Size)
        throw std::out_of_range("Index out of range.");

    if (_slot.size() < 2 || !std::isdigit(_slot[1]))
        throw std::invalid_argument("Slot name format invalid. Expected format like 't0', 'u1', 's2', or 'b0'.");

    int slotNum = std::stoi(_slot.substr(1));
    char typeChar = _slot[0];

    D3D12_ROOT_PARAMETER& param = params_[_index];
    param.ShaderVisibility = _visibility;

    switch (typeChar)
    {
    case 't':
    case 'u':
    case 's':
    {
        auto& range = ranges_.emplace_back();
        range.BaseShaderRegister = slotNum;
        range.NumDescriptors = 1;
        range.RegisterSpace = 0;
        range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

        switch (typeChar)
        {
        case 't': range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; break;
        case 'u': range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV; break;
        case 's': range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER; break;
        }

        param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        param.DescriptorTable.NumDescriptorRanges = 1;
        param.DescriptorTable.pDescriptorRanges = &range;
        break;
    }

    case 'b':
        param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
        param.Descriptor.ShaderRegister = slotNum;
        param.Descriptor.RegisterSpace = 0;
        break;

    default:
        throw std::invalid_argument("Invalid slot type. Must start with 't', 'u', 's', or 'b'.");
    }

    slotMap_[_slot] = _index;

    return *this;
}