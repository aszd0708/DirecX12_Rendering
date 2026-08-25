#pragma once

#define WIN32_LEAN_AND_MEAN

#include "Types.h"
#include "Define.h"

// STL
#include <memory>
#include <iostream>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <concepts>
#include <type_traits>
#include <string>
#include <optional>
#include <assert.h>
#include <stdexcept>
#include <concepts>
using namespace std;

// WIN
#include <windows.h>

// DirectX 12
#include <d3d12.h>
#include <d3dx12.h>
#include <comdef.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>
using namespace DirectX;
using namespace Microsoft::WRL;


inline void ThrowIfFailed(HRESULT hr)
{
    CHECK(hr);
}

// STB
#include "stb_image.h"

// Libs
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex/DirectXTex.lib")
#endif

// Managers
#include "Game.h"
#include "Graphics.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "SceneManager.h"

// Core
#include "Component.h"
#include "GameObject.h"
#include "Transform.h"
#include "Renderer.h"

// Geometry
#include "VertexData.h"
#include "Geometry.h"
#include "GeometryHelper.h"
#include "Scene.h"

// ImGUI
#include "imgui.h"