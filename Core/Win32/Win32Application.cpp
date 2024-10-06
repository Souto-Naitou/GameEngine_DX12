#include "Win32Application.h"
#include <cassert>
#include <Externals/imgui/imgui.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void Win32Application::Initialize()
{
	HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr) && "初期化失敗");

    wc_.lpfnWndProc = Win32Application::WindowProcedure;
    wc_.lpszClassName = L"DXWindowClass";
    wc_.hInstance = GetModuleHandle(nullptr);
    wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);
    RegisterClass(&wc_);
}

void Win32Application::Finalize()
{
	CloseWindow(hwnd_);
	CoUninitialize();
}

void Win32Application::ShowWnd()
{
    RECT wrc = { 0,0,kClientWidth, kClientHeight };
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
	hwnd_ = CreateWindow(
		wc_.lpszClassName,		// 利用するクラス名
		L"CG2",					// タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	// よく見るウィンドウスタイル
		CW_USEDEFAULT,			// 表示X座標(ウィンドウ出現位置？)
		CW_USEDEFAULT,			// 表示Y座標
		wrc.right - wrc.left,	// ウィンドウ横幅
		wrc.bottom - wrc.top,	// ウィンドウ縦幅
		nullptr,				// 親ウィンドウハンドル(子ウィンドウ作るときに使うかも？)
		nullptr,				// メニューハンドル
		wc_.hInstance,			// インスタンスハンドル	
		nullptr					// オプション
	);
	ShowWindow(hwnd_, SW_SHOW);
}

UINT Win32Application::GetMsg()
{
	// Windowにメッセージが来てたら最優先で処理させる
	if (PeekMessage(&msg_, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg_);	// メッセージを解釈？
		DispatchMessage(&msg_);	// デキュー的な？
	}
	return msg_.message;
}

LRESULT CALLBACK Win32Application::WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}

	// メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
	case WM_DESTROY:	// ウィンドウが破壊された
		// OSに対して、アプリ終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
