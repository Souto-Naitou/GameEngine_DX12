#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <Windows.h>
#include <wrl.h>

class Input
{
public:
    void Initialize(HINSTANCE _hInstance, HWND _hwnd);
    void Update();

    bool PushKey(BYTE _keyNumber) const;
    bool TriggerKey(BYTE _keyNumber) const;

private:
    Microsoft::WRL::ComPtr<IDirectInput8> directInput_ = nullptr;
    Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard_ = nullptr;
    BYTE key_[256] = {};
    BYTE keyPre_[256] = {};
};