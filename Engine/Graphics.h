#pragma once

class Graphics
{
	DECLARE_SINGLE(Graphics);

public:
	~Graphics();
	void Init(HWND hwnd);

private:
	void CreateCommandQueue();
	void CreateCommandAllocator();
	void CreateCommandList();

	void CreateFactory();
	void CreateSwapChain();
	void CreateBackBufferRTV();
	void CreateFence();

public:
	void RenderBegin();
	void RenderEnd();

private:
	void SetResourceBarrier(bool isBegin);

private:
	HWND _hwnd = {};

	ComPtr<ID3D12Device> _device;
	ComPtr<IDXGIFactory4> _factory;
	ComPtr<ID3D12CommandQueue> _commandQueue;
	ComPtr<ID3D12CommandAllocator> _commandAllocator;
	ComPtr<ID3D12GraphicsCommandList> _commandList;

	ComPtr<IDXGISwapChain3> _swapChain;
	ComPtr<ID3D12Resource> _renderTargets[SWAP_CHAIN_BUFFER_COUNT];

	ComPtr<ID3D12DescriptorHeap> _rtvHeap;
	uint32 _rtvDescriptorSize = 0;

	ComPtr<ID3D12Fence> _fence;
	uint64 _fenceValue = 0;
	HANDLE _fenceEvent = nullptr;

private:
	const float CLEAR_COLOR[4] = {0.5f, 0.5f, 0.5f, 1.0f};
};
