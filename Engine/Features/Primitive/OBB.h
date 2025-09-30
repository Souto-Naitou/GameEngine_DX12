#pragma once

#include <Vector3.h>
#include <Matrix4x4.h>

#include <Features/Line/Line.h>
#include <Features/Line/LineSystem.h>
#include <Features/GameEye/GameEye.h>

#include <array>
#include <memory>

class OBB
{
public:
    OBB() = default;
    ~OBB();

    void Initialize();
    void Update();
    void Draw();


public: /// Setter
    void SetCenter(const Vector3& _center) { center_ = _center; }
    void SetOrientations(const Vector3& _orientation1, const Vector3& _orientation2, const Vector3& _orientation3)
    {
        orientations_[0] = _orientation1;
        orientations_[1] = _orientation2;
        orientations_[2] = _orientation3;
    }
    void SetOrientations(const Matrix4x4& _rotateMatrix)
    {
        orientations_[0] = Vector3(_rotateMatrix.m[0][0], _rotateMatrix.m[1][0], _rotateMatrix.m[2][0]);
        orientations_[1] = Vector3(_rotateMatrix.m[0][1], _rotateMatrix.m[1][1], _rotateMatrix.m[2][1]);
        orientations_[2] = Vector3(_rotateMatrix.m[0][2], _rotateMatrix.m[1][2], _rotateMatrix.m[2][2]);

        rotateMatrix_ = _rotateMatrix;
    }
    void SetSize(const Vector3& _size) { size_ = _size; }
    void SetGameEye(GameEye** _eye) { pGameEye = _eye; ModifyGameEye(_eye); }


public: /// Getter
    const Vector3& GetCenter() const { return center_; }
    const Vector3& GetOrientation(int _index) const { return orientations_[_index]; }
    const Vector3& GetSize() const { return size_; }
    const Matrix4x4& GetRotateMatrix() const { return rotateMatrix_; }


private:
    Vector3 center_ = {}; // 中心点
    Vector3 orientations_[3] = {}; // 座標軸。正規化・直交必須
    Vector3 size_ = {}; // 座標軸方向の長さの半分。中心から面までの距離

    Matrix4x4 rotateMatrix_ = {};

    Line lines_ = { 12 }; // 線


private:
    void ModifyGameEye(GameEye** _eye);


private:
    LineSystem* pLineSystem = nullptr;
    GameEye** pGameEye = nullptr;
};