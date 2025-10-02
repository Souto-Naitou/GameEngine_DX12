#pragma once

#include "math/NiVec2.h" // NiVec2

#define WIN32_LEAN_AND_MEAN
#include <Windows.h> // HWND, UINT, WPARAM, LPARAM


/// UI入力クラス
class NiGui_Input
{
public:
    /// マウスデータ
    struct MouseData
    {
        NiVec2 pos;
        bool isLeft;
        bool isLeftPre;
        bool isRight;
        bool isRightPre;
        bool isMiddle;
        bool isMiddlePre;
        NiVec2 triggeredPos;
        NiVec2 differencePos;
    };

public: /// コンストラクタとデストラクタ
    NiGui_Input() = default;
    ~NiGui_Input() = default;


public: /// 一般

    // 初期化
    void Initialize();

    // 入力データ更新
    // データを取得する前に呼び出してください。
    void Update();

    // ウィンドウプロシージャハンドラ
    void WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


public: // 入力データ取得

    // 左クリックプレス
    bool PressLeft() const { return mouseData_.isLeft; }
    // 左クリックトリガー
    bool TriggerLeft() const { return mouseData_.isLeft && !mouseData_.isLeftPre; }
    // 左クリックリリース
    bool ReleaseLeft() const { return !mouseData_.isLeft && mouseData_.isLeftPre; }

    // 右クリックプレス
    bool PressRight() const { return mouseData_.isRight; }
    // 右クリックトリガー
    bool TriggerRight() const { return mouseData_.isRight && !mouseData_.isRightPre; }
    // 右クリックリリース
    bool ReleaseRight() const { return !mouseData_.isRight && mouseData_.isRightPre; }

    // 中クリックプレス
    bool PressMiddle() const { return mouseData_.isMiddle; }
    // 中クリックトリガー
    bool TriggerMiddle() const { return mouseData_.isMiddle && !mouseData_.isMiddlePre; }
    // 中クリックリリース
    bool ReleaseMiddle() const { return !mouseData_.isMiddle && mouseData_.isMiddlePre; }

    // マウス座標
    NiVec2 GetMousePos() const { return mouseData_.pos; }

    // トリガーした座標
    NiVec2 GetTriggeredPos() const { return mouseData_.triggeredPos; }

    // マウスの移動量 ( 前フレームの座標からの差分 )
    NiVec2 GetDifferencePos() const { return mouseData_.differencePos; }


private:
    // マウスデータ
    MouseData mouseData_ = {};

    HWND hWnd_ = nullptr;

    bool isLeftFromMsg = false;
    bool isRightFromMsg = false;
    bool isMiddleFromMsg = false;
};