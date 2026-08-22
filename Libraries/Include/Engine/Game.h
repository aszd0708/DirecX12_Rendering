#pragma once

struct GameDesc
{
	shared_ptr<class SceneBuilder> _scene;
	wstring appName = L"DX12_Study";
	HINSTANCE hInstance = 0;
	HWND hWnd = 0;
	float width = 800;
	float height = 600;
	bool vsync = false;
	bool windowed = true;
};

class Game
{
	DECLARE_SINGLE(Game);
public:
	WPARAM Run(GameDesc& desc);

	GameDesc& GetGameDesc() { return _desc; }

private:
	ATOM MyRegisterClass();
	BOOL InitInstance(int cmdShow);

	void Update();
	void ShowFPS();

	static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	
private:
	GameDesc _desc;
};
