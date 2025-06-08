#pragma once

#include <array>

#include <Vector3.h>
#include <Features/Line/Line.h>
#include <Features/Line/LineSystem.h>
#include <Features/GameEye/GameEye.h>

#include <memory>

class AABB
{
public:
    AABB() = default;
    ~AABB();

    void Initialize();
    void Draw();


public: /// Setter
    void SetMinMax(const Vector3& _min, const Vector3& _max)
    {
        min_ = _min;
        max_ = _max;
    }
    void SetGameEye(GameEye** _eye) { pGameEye = _eye; ModifyGameEye(_eye); }


public: /// Getter
    const Vector3& GetMin() const { return min_; }
    const Vector3& GetMax() const { return max_; }


private:
    Vector3 min_; // 最小点
    Vector3 max_; // 最大点
    std::unique_ptr<Line> lines = nullptr; // 線


private:
    void ModifyGameEye(GameEye** _eye);


private:
    LineSystem* pLineSystem = nullptr;
    GameEye** pGameEye = nullptr;
};