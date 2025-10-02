#include "NiGui_Input.h"

#include <WinUser.h> // WM_LBUTTONDOWN, WM_LBUTTONUP, WM_RBUTTONDOWN, WM_RBUTTONUP, WM_MBUTTONDOWN, WM_MBUTTONUP

void NiGui_Input::Initialize()
{
    mouseData_ = {};
}

void NiGui_Input::Update()
{
    /// 前フレームの状態を保存
    mouseData_.isLeftPre = mouseData_.isLeft;
    mouseData_.isRightPre = mouseData_.isRight;
    mouseData_.isMiddlePre = mouseData_.isMiddle;

    /// マウスの状態を取得
    mouseData_.isLeft = isLeftFromMsg;
    mouseData_.isRight = isRightFromMsg;
    mouseData_.isMiddle = isMiddleFromMsg;

    /// マウスの座標を取得
    POINT point;
    // 座標を取得
    GetCursorPos(&point);
    // クライアント座標に変換
    if(hWnd_ != nullptr)
    {
        ScreenToClient(hWnd_, &point);
    }

    NiVec2 prePos = mouseData_.pos;

    mouseData_.pos = NiVec2(static_cast<float>(point.x), static_cast<float>(point.y));

    mouseData_.differencePos = mouseData_.pos - prePos;

    /// トリガーした座標を保存
    if (TriggerLeft() || TriggerMiddle() || TriggerRight())
    {
        mouseData_.triggeredPos = mouseData_.pos;
    }
}

void NiGui_Input::WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    hWnd_ = hWnd;

    switch(msg)
    {
    case WM_LBUTTONDOWN:
        isLeftFromMsg = true;
        break;
    case WM_LBUTTONUP:
        isLeftFromMsg = false;
        break;
    case WM_RBUTTONDOWN:
        isRightFromMsg = true;
        break;
    case WM_RBUTTONUP:
        isRightFromMsg = false;
        break;
    case WM_MBUTTONDOWN:
        isMiddleFromMsg = true;
        break;
    case WM_MBUTTONUP:
        isMiddleFromMsg = false;
        break;
    }

    return;
}
