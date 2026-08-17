#include "pch.h"
#include "Graphics.h"

void Graphics::Init(HWND hwnd)
{
	_hwnd = hwnd;
	// TODO: DirectX 12 초기화 구현
	// 1. D3D12CreateDevice
	// 2. CommandQueue, CommandAllocator, CommandList 생성
	// 3. SwapChain 생성
	// 4. Descriptor Heap(RTV, DSV, CBV/SRV/UAV) 생성 및 BackBuffer RTV 생성
	// 5. Fence (GPU-CPU 동기화) 객체 생성

	
	HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&_device));
	CHECK(hr);

	CreateCommandQueue();
	CreateCommandAllocator();
	CreateCommandList();

}

void Graphics::RenderBegin()
{
	// TODO: 렌더링 시작 구현
	// 1. CommandAllocator, CommandList Reset
	// 2. ResourceBarrier (PRESENT -> RENDER_TARGET)
	// 3. RenderTargetView / DepthStencilView 바인딩 및 Clear
}

void Graphics::RenderEnd()
{
	// TODO: 렌더링 종료 및 화면 표시 구현
	// 1. ResourceBarrier (RENDER_TARGET -> PRESENT)
	// 2. CommandList Close & CommandQueue 실행
	// 3. SwapChain Present
	// 4. Fence를 이용한 동기화 대기
}

void Graphics::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC desc;
	ZeroMemory(&desc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;

	_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&_commandQueue));
}

void Graphics::CreateCommandAllocator()
{
	_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_commandAllocator));
}

void Graphics::CreateCommandList()
{
	_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocator.Get(), nullptr, IID_PPV_ARGS(&_commandList));
}

void Graphics::CreateFactory()
{
	ComPtr<IDXGIFactory2> factory;
	CreateDXGIFactory2(0, IID_PPV_ARGS(&factory));
	factory.As(&_factory);
}

void Graphics::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.BufferCount = SwapChainCount;
	desc.Width = GAME->GetGameDesc().width;
	desc.Height = GAME->GetGameDesc().height;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.SampleDesc.Count = 1;

	ComPtr<IDXGISwapChain1> swapChain;
	ThrowIfFailed(_factory->CreateSwapChainForHwnd(_commandQueue.Get(), _hwnd, &desc, nullptr, nullptr, swapChain.GetAddressOf()));
	
	swapChain.As(&_swapChain);
}