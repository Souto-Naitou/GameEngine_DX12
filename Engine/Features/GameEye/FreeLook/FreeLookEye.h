#pragma once

#include <Features/GameEye/GameEye.h>
#include <Features/Input/Input.h>
#include <Quaternion.h>

class FreeLookEye : public GameEye
{
public:
    FreeLookEye();
    ~FreeLookEye() override;

    void Update() override;


private:
    float moveSpeed_ = 0.1f;
    Quaternion rotate_ = Quaternion::Identity();
    POINT currCursorPos_ = {};
    POINT prevCursorPos_ = {};


private:
    void CatchMoveCommands();
    void CatchRotateCommands();
    void DebugWindow() override;


private:
    Input* pInput_ = nullptr;
};