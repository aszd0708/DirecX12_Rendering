#include "pch.h"
#include "Game.h"
#include "SceneBuilder.h"
#include "Graphics.h"

WPARAM Game::Run(GameDesc& desc)
{
	_desc = desc;

	MyRegisterClass();

	if (!InitInstance(SW_SHOWNORMAL))
		return FALSE;

	// Graphics 초기화
	GRAPHICS->Init(_desc.hWnd);

	// Manager 초기화
	TIME->Init();
	INPUT->Init(_desc.hWnd);

	// Scene 초기화
	_desc._scene->Init();

	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update();
		}
	}

	return msg.wParam;
}

ATOM Game::MyRegisterClass()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _desc.hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _desc.appName.c_str();
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	return RegisterClassExW(&wcex);
}

BOOL Game::InitInstance(int cmdShow)
{
	RECT windowRect = { 0, 0, static_cast<LONG>(_desc.width), static_cast<LONG>(_desc.height) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	_desc.hWnd = CreateWindowW(_desc.appName.c_str(), _desc.appName.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, _desc.hInstance, nullptr);

	if (!_desc.hWnd)
		return FALSE;

	ShowWindow(_desc.hWnd, cmdShow);
	UpdateWindow(_desc.hWnd);

	return TRUE;
}

void Game::Update()
{
	TIME->Update();
	INPUT->Update();

	ShowFPS();

	GRAPHICS->RenderBegin();

	_desc._scene->Update();
	_desc._scene->Render();

	GRAPHICS->RenderEnd();
}

void Game::ShowFPS()
{
	uint32 fps = TIME->GetFps();

	WCHAR text[100] = L"";
	::wsprintf(text, L"FPS : %d", fps);

	::SetWindowText(_desc.hWnd, text);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Game::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_SIZE:
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}

	return 0;
}