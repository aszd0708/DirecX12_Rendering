#include "pch.h"
#include "Main.h"
#include "Engine/Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	GameDesc desc;
	desc.appName = L"DX12_Study";
	desc.hInstance = hInstance;
	desc.vsync = false;
	desc.hWnd = NULL;
	desc.width = 1280;
	desc.height = 720;

	GAME->Run(desc);

	return 0;
}