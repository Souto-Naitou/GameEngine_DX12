#pragma once

class IObjectSystem
{
public:
    // dtor
    virtual ~IObjectSystem() = default;
    virtual void Initialize() = 0;
    virtual void PrepareDraw() = 0;
};