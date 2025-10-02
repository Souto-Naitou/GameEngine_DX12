#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <wrl.h>
#include <cstdint>
#include <Vector2.h>

class Input
{
public:
    enum class MouseNum
    {
        Left = 0,
        Right = 1,
        Center = 2,
    };

    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;
    Input(Input&&) = delete;
    Input& operator=(Input&&) = delete;

    static Input* GetInstance()
    {
        static Input instance;
        return &instance;
    }

    void        Initialize(HINSTANCE _hInstance, HWND _hwnd);
    void        Update();

    void        Enable(bool _flag);

    void        SetDeadZoneRange(float deadZoneRange) { deadZoneRange_ = deadZoneRange; }

    bool        PushKey(BYTE _keyNumber) const;
    bool        PushKeyC(char _key) const;
    bool        TriggerKey(BYTE _keyNumber) const;
    bool        TriggerKeyC(char _key) const;
    Vector2     GetLeftStickPosition() const;
    Vector2     GetRightStickPosition() const;

    bool        PushMouse(MouseNum _mouseNum) const;
    bool        TriggerMouse(MouseNum _mouseNum) const;
    int32_t     GetWheelDelta() const;
    IDirectInput8* GetDirectInput() const { return directInput_.Get(); }
    IDirectInputDevice8** GetPad() { return pad_.GetAddressOf(); }


private:
    Input() = default;
    ~Input() = default;
    
    // Internal functions
    static BOOL CALLBACK EnumJoystickCallback(const DIDEVICEINSTANCE* pdidInstance, void* context);
    BYTE GetKeyNumber(char _key) const;
    void MapInputData();
    void InitializePad(HWND hwnd);
    void UpdatePad();
    void UpdateDeviceState(IDirectInputDevice8* pDevice, LPVOID out_state, size_t sizeState);

    // State
    bool isEnable_ = true;

    // Config
    const int32_t kStickRange = 1000; // ジョイスティックの範囲

    // DirectInput data
    Microsoft::WRL::ComPtr<IDirectInput8> directInput_ = nullptr;
    Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard_ = nullptr;
    Microsoft::WRL::ComPtr<IDirectInputDevice8> mouse_ = nullptr;
    Microsoft::WRL::ComPtr<IDirectInputDevice8> pad_ = nullptr;
    DIMOUSESTATE2 mouseState_ = {};
    DIJOYSTATE2 padState_ = {};

    // Logical input data
    BYTE    key_[256]           = {};
    BYTE    keyPre_[256]        = {};
    bool    leftClick_          = false;
    bool    leftClickPre_       = false;
    bool    rightClick_         = false;
    bool    rightClickPre_      = false;
    int32_t wheelDelta_         = 0;
    Vector2 leftStickPosition_  = { 0.0f, 0.0f };
    Vector2 rightStickPosition_ = { 0.0f, 0.0f };
    bool    buttons_[32]        = {}; // ボタンの状態
    bool    buttonsPre_[32]     = {}; // 前回のボタン状態
    float   deadZoneRange_      = 0.1f;  // ジョイスティックのデッドゾーン範囲
};