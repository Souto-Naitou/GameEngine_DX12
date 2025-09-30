#pragma once

class IEntityStats
{
public:
    virtual ~IEntityStats() = default;
    virtual void OnCollision(const IEntityStats* status) = 0;
};