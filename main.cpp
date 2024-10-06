#include <Windows.h>
#include <DirectXCommon.h>
#include <Win32Application.h>
#include <Input.h>
#include <memory>

int _stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Win32Application* pWin32App = nullptr;
	DirectXCommon* pDirectXCommon = nullptr;
	std::unique_ptr<Input> pInput = nullptr;

	pWin32App = Win32Application::GetInstance();
	pDirectXCommon = DirectXCommon::GetInstance();
	pInput = std::make_unique<Input>();

	pWin32App->Initialize();
	pWin32App->ShowWnd();

	pDirectXCommon->Initialize();
	pInput->Initialize(pWin32App->GetHInstance(), pWin32App->GetHwnd());


	while (pWin32App->GetMsg() != WM_QUIT)
	{
		// 入力の更新
		pInput->Update();

		/// デバッグ用
		if (pInput->TriggerKey(DIK_SPACE))
		{
			OutputDebugString(L"とりがー\n");
		}
		if (pInput->PushKey(DIK_RETURN))
		{
			OutputDebugString(L"ぷっしゅちゅー\n");
		}
	}

	pDirectXCommon->Finalize();
	pWin32App->Finalize();

	return 0;
}