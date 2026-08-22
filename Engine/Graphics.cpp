#include "pch.h"
#include "Graphics.h"

Graphics::~Graphics()
{
	::CloseHandle(_fenceEvent);
}

void Graphics::Init(HWND hwnd)
{
	_hwnd = hwnd;
	// TODO: DirectX 12 초기화 구현
	// 1. D3D12CreateDevice
	// 2. CommandQueue, CommandAllocator, CommandList 생성
	// 3. SwapChain 생성
	// 4. Descriptor Heap(RTV, DSV, CBV/SRV/UAV) 생성 및 BackBuffer RTV 생성
	// 5. Fence (GPU-CPU 동기화) 객체 생성

	
	ThrowIfFailed(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&_device)));

	CreateCommandQueue();
	CreateCommandAllocator();
	CreateCommandList();

	CreateFactory();
	CreateSwapChain();
	CreateBackBufferRTV();
	CreateFence();
}

void Graphics::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC desc;
	ZeroMemory(&desc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;

	ThrowIfFailed(_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&_commandQueue)));
}

void Graphics::CreateCommandAllocator()
{
	ThrowIfFailed(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_commandAllocator)));
}

void Graphics::CreateCommandList()
{
	ThrowIfFailed(_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocator.Get(), nullptr, IID_PPV_ARGS(&_commandList)));
	_commandList->Close();

	_viewport.TopLeftX = 0;
	_viewport.TopLeftY = 0;
	_viewport.Width = SCREEN_WIDTH;
	_viewport.Height = SCREEN_HEIGHT;
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;

	_rect.left = 0;
	_rect.top = 0;
	_rect.right = SCREEN_WIDTH;
	_rect.bottom = SCREEN_HEIGHT;
}

void Graphics::CreateFactory()
{
#ifdef _DEBUG
	ThrowIfFailed(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&_factory)));
#else
	ThrowIfFailed(CreateDXGIFactory2(0, IID_PPV_ARGS(&_factory)));
#endif // _DEBUG
}

void Graphics::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.BufferCount = SWAP_CHAIN_BUFFER_COUNT;
	desc.Width = GAME->GetGameDesc().width;
	desc.Height = GAME->GetGameDesc().height;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.SampleDesc.Count = 1;

	ComPtr<IDXGISwapChain1> swapChain;
	ThrowIfFailed(_factory->CreateSwapChainForHwnd(_commandQueue.Get(), _hwnd, &desc, nullptr, nullptr, swapChain.GetAddressOf()));
	
	swapChain.As(&_swapChain);

	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
	{
		ThrowIfFailed(_swapChain->GetBuffer(i, IID_PPV_ARGS(&_renderTargets[i])));
	}
}

void Graphics::CreateBackBufferRTV()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = 2;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	ThrowIfFailed(_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_rtvHeap)));

	_rtvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_CPU_DESCRIPTOR_HANDLE handle = _rtvHeap->GetCPUDescriptorHandleForHeapStart();
	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
	{
		_device->CreateRenderTargetView(_renderTargets[i].Get(), nullptr, handle);
		handle.ptr += _rtvDescriptorSize;
	}
}

void Graphics::CreateFence()
{
	ThrowIfFailed(_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)));
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void Graphics::RenderBegin()
{
	// TODO: 렌더링 시작 구현
	// 1. CommandAllocator, CommandList Reset
	// 2. ResourceBarrier (PRESENT -> RENDER_TARGET)
	// 3. RenderTargetView / DepthStencilView 바인딩 및 Clear

	_commandAllocator->Reset();
	_commandList->Reset(_commandAllocator.Get(), nullptr);
	SetResourceBarrier(true);

	uint32 backIndex = _swapChain->GetCurrentBackBufferIndex();
	D3D12_CPU_DESCRIPTOR_HANDLE descHandler = _rtvHeap->GetCPUDescriptorHandleForHeapStart();
	int offset = backIndex * _rtvDescriptorSize;
	descHandler.ptr += offset;
	_commandList->OMSetRenderTargets(1, &descHandler, false, nullptr);

	_commandList->ClearRenderTargetView(descHandler, CLEAR_COLOR, 0, nullptr);

	_commandList->RSSetViewports(1, &_viewport);
	_commandList->RSSetScissorRects(1, &_rect);
}

void Graphics::RenderEnd()
{
	// TODO: 렌더링 종료 및 화면 표시 구현
	// 1. ResourceBarrier (RENDER_TARGET -> PRESENT)
	// 2. CommandList Close & CommandQueue 실행
	// 3. SwapChain Present
	// 4. Fence를 이용한 동기화 대기

	SetResourceBarrier(false);
	
	_commandList->Close();
	ID3D12CommandList* cmdLists[] = { _commandList.Get() };
	_commandQueue->ExecuteCommandLists(1, cmdLists);

	_swapChain->Present(0,0);
	
	_fenceValue++;
	_commandQueue->Signal(_fence.Get(), _fenceValue);

	if (_fence->GetCompletedValue() < _fenceValue)
	{
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent);

		WaitForSingleObject(_fenceEvent, INFINITE);
	}
}

void Graphics::SetResourceBarrier(bool isBegin)
{
	uint32 backIndex = _swapChain->GetCurrentBackBufferIndex();

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = _renderTargets[backIndex].Get();

	if (isBegin)
	{
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	}
	else
	{

		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	}

	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	_commandList->ResourceBarrier(1, &barrier);
}
