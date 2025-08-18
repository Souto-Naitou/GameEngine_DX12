#include "Input.h"

#include <cassert>
#include <stdexcept>

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
        _hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
    );
    assert(SUCCEEDED(hr));

    // キーボード情報の取得開始
    keyboard_->Acquire();

    // マウスデバイスの生成
    hr = directInput_->CreateDevice(GUID_SysMouse, &mouse_, NULL);
    assert(SUCCEEDED(hr));

    // 入力データ形式のセット
    hr = mouse_->SetDataFormat(&c_dfDIMouse2);
    assert(SUCCEEDED(hr));

    // 排他制御レベルのセット
    hr = mouse_->SetCooperativeLevel(
        _hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
    );

    // マウス情報の取得開始
    mouse_->Acquire();

    assert(SUCCEEDED(hr));

    this->InitializePad(_hwnd);
}

void Input::Update()
{
    // 配列をコピー
    memcpy(keyPre_, key_, 256);

    this->UpdateDeviceState(keyboard_.Get(), key_, sizeof(key_));

    // マウスの入力情報を取得
    this->UpdateDeviceState(mouse_.Get(), &mouseState_, sizeof(mouseState_));

    // マウスの状態を更新
    this->UpdateDeviceState(pad_.Get(), &padState_, sizeof(padState_));

    // ゲームパッドの更新
    this->UpdatePad();

    // デシリアライズ
    this->MapInputData();
}

void Input::Enable(bool _flag)
{
    isEnable_ = _flag;
}

bool Input::PushKey(BYTE _keyNumber) const
{
    // 指定キーを押して入ればtrueを返す
    if (key_[_keyNumber])
    {
        return true;
    }
    return false;
}

bool Input::PushKeyC(char _key) const
{
    // 指定キーを押して入ればtrueを返す
    if(key_[GetKeyNumber(_key)])
    {
        return true;
    }

    return false;
}

bool Input::TriggerKey(BYTE _keyNumber) const
{
    // キーが押された瞬間ならtrueを返す
    if(key_[_keyNumber] && !keyPre_[_keyNumber])
    {
        return true;
    }

    return false;
}

bool Input::TriggerKeyC(char _key) const
{
    // キーが押された瞬間ならtrueを返す
    BYTE keyNumber = GetKeyNumber(_key);
    if(key_[keyNumber] && !keyPre_[keyNumber])
    {
        return true;
    }

    return false;
}

Vector2 Input::GetLeftStickPosition() const
{
    return leftStickPosition_;
}

Vector2 Input::GetRightStickPosition() const
{
    return rightStickPosition_;
}

bool Input::PushMouse(MouseNum _mouseNum) const
{
    if (_mouseNum == MouseNum::Left)
    {
        if (leftClick_)
        {
            return true;
        }
    }
    else if (_mouseNum == MouseNum::Right)
    {
        if (rightClick_)
        {
            return true;
        }
    }

    return false;
}

bool Input::TriggerMouse(MouseNum _mouseNum) const
{
    if (_mouseNum == MouseNum::Left)
    {
        if (leftClick_ && !leftClickPre_)
        {
            return true;
        }
    }
    else if (_mouseNum == MouseNum::Right)
    {
        if (rightClick_ && !rightClickPre_)
        {
            return true;
        }
    }

    return false;
}

int32_t Input::GetWheelDelta() const
{
    return wheelDelta_;
}

BYTE Input::GetKeyNumber(char _key) const
{
    // キーの文字からキー番号を取得
    return static_cast<BYTE>(MapVirtualKey(_key, MAPVK_VK_TO_VSC));
}

void Input::MapInputData()
{
    leftClickPre_ = leftClick_;
    rightClickPre_ = rightClick_;

    if (!isEnable_ && !leftClick_ && !rightClick_)
    {
        leftClick_ = false;
        rightClick_ = false;
        wheelDelta_ = 0;
        return;
    }

    // 左クリックの入力情報を取得
    if (mouseState_.rgbButtons[0] & 0x80)
    {
        leftClick_ = true;
    }
    else
    {
        leftClick_ = false;
    }

    // 右クリックの入力情報を取得
    if (mouseState_.rgbButtons[1] & 0x80)
    {
        rightClick_ = true;
    }
    else
    {
        rightClick_ = false;
    }

    wheelDelta_ = mouseState_.lZ;
}

void Input::InitializePad(HWND hwnd)
{
    HRESULT hr = directInput_->EnumDevices(
        DI8DEVTYPE_GAMEPAD, EnumJoystickCallback, (void*)this, DIEDFL_ATTACHEDONLY
    );
    assert(SUCCEEDED(hr));

    if (!pad_) return;

    hr = pad_->SetDataFormat(&c_dfDIJoystick2);
    assert(SUCCEEDED(hr));

    // 軸を絶対値モードに設定
    {
        DIPROPDWORD prop;
        ZeroMemory(&prop, sizeof(prop));
        prop.diph.dwSize = sizeof(prop);
        prop.diph.dwHeaderSize = sizeof(prop.diph);
        prop.diph.dwHow = DIPH_DEVICE;
        prop.diph.dwObj = 0;
        prop.dwData = DIPROPAXISMODE_ABS;
        hr = pad_->SetProperty(DIPROP_AXISMODE, &prop.diph);
    }
    assert(SUCCEEDED(hr));

    // ジョイスティックの範囲を設定
    {
        DIPROPRANGE prop;
        ZeroMemory(&prop, sizeof(prop));
        prop.diph.dwSize = sizeof(prop);
        prop.diph.dwHeaderSize = sizeof(prop.diph);
        prop.diph.dwHow = DIPH_BYOFFSET;
        prop.diph.dwObj = DIJOFS_X;
        prop.lMin = -kStickRange;
        prop.lMax = kStickRange;
        hr = pad_->SetProperty(DIPROP_RANGE, &prop.diph);
        // Y軸の範囲も同様に設定
        prop.diph.dwObj = DIJOFS_Y;
        hr = pad_->SetProperty(DIPROP_RANGE, &prop.diph);
    }
    assert(SUCCEEDED(hr));

    // ジョイスティックの排他制御レベルを設定
    hr = pad_->SetCooperativeLevel(
        hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE
    );
    assert(SUCCEEDED(hr));

    // ジョイスティックの情報の取得を開始
    hr = pad_->Acquire();
    assert(SUCCEEDED(hr));
}

void Input::UpdatePad()
{
    if (!pad_) return;
    leftStickPosition_.x = padState_.lX / static_cast<float>(kStickRange); // X軸の値を-1.0fから1.0fに変換
    leftStickPosition_.y = padState_.lY / static_cast<float>(kStickRange); // Y軸の値を-1.0fから1.0fに変換
    rightStickPosition_.x = padState_.lRx / static_cast<float>(kStickRange); // 右スティックX軸
    rightStickPosition_.y = padState_.lRy / static_cast<float>(kStickRange); // 右スティックY軸

    // ボタンの状態を更新
    for (int i = 0; i < 32; ++i)
    {
        if (padState_.rgbButtons[i] & 0x80)
        {
            buttons_[i] = true; // ボタンが押されている
        }
        else
        {
            buttons_[i] = false; // ボタンが離されている
        }
    }
}

void Input::UpdateDeviceState(IDirectInputDevice8* pDevice, LPVOID out_state, size_t sizeState)
{
    if (!pDevice) return;

    // デバイスの状態
    HRESULT hr = pDevice->Poll();

    if (FAILED(hr))
    {
        // デバイスが応答しない場合は再取得
        hr = pDevice->Acquire();
        hr = pDevice->Poll();
    }

    if (FAILED(hr)) return;
    
    hr = pDevice->GetDeviceState(static_cast<int>(sizeState), out_state);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to get device state.");
    }
}

int Input::EnumJoystickCallback(const DIDEVICEINSTANCE* pdidInstance, void* context)
{
    auto pInput = static_cast<Input*>(context);
    auto directInput = pInput->GetDirectInput();
    
    if (FAILED(directInput->CreateDevice(pdidInstance->guidInstance, pInput->GetPad(), NULL)))
    {
        return DIENUM_CONTINUE; // エラーが発生した場合は次のデバイスへ
    }
    return DIENUM_STOP; // ジョイスティックが見つかったので列挙を停止
}
