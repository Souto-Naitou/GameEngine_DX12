#include <Input.h>

#include <cassert>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

void Input::Initialize(HINSTANCE _hInstance, HWND _hwnd)
{
    HRESULT hr = DirectInput8Create(
        _hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr
    );
    assert(SUCCEEDED(hr));

    // キーボードデバイスの生成
    hr = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
    assert(SUCCEEDED(hr));

    // 入力データ形式のセット
    hr = keyboard_->SetDataFormat(&c_dfDIKeyboard); // 標準形式
    assert(SUCCEEDED(hr));

    // 排他制御レベルのセット
    hr = keyboard_->SetCooperativeLevel(
        _hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
    );
    assert(SUCCEEDED(hr));
}