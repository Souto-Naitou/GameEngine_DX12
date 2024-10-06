#pragma once
#include <Windows.h>
#include <stdint.h>

class Win32Application
{
public:
    static const uint32_t kClientWidth = 1280u;
    static const uint32_t kClientHeight = 720u;

    Win32Application(const Win32Application&) = delete;
    Win32Application(const Win32Application&&) = delete;
    Win32Application& operator=(const Win32Application&) = delete;
    Win32Application& operator=(const Win32Application&&) = delete;

    static Win32Application* GetInstance() { static Win32Application instance;return &instance; };

    HWND GetHwnd() const { return hwnd_; }
    HINSTANCE GetHInstance() const { return wc_.hInstance; }

    void Initialize();
    void Finalize();
    void ShowWnd();
    UINT GetMsg();

private:
    Win32Application() = default;
    ~Win32Application() = default;

    static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    WNDCLASS    wc_         = {};
    HWND        hwnd_       = {};
    MSG         msg_        = {};
};