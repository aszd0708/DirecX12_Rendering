#include "pch.h"
#include "Main.h"
#include "Engine/Game.h"
#include "TriagleDemo.h"
#include "MeshDemo.h"
#include "InstancingDemo.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	GameDesc desc;
	desc._scene = make_shared<InstancingDemo>("InstancingDemo");
	desc.appName = L"DX12_Study";
	desc.hInstance = hInstance;
	desc.vsync = false;
	desc.hWnd = NULL;
	desc.width = 800;
	desc.height = 600;

	GAME->Run(desc);

	return 0;
}