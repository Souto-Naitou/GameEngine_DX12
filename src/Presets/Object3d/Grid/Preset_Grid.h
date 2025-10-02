#pragma once
#include <memory>
#include <Features/Object3d/Object3d.h>

namespace presets::grid
{
    std::unique_ptr<Object3d> Create(IModel* pModel);
}