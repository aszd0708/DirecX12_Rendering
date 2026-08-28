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

	void WaitForGPU();

private:
	void SetResourceBarrier(bool isBegin);

public:
	ComPtr<ID3D12Device> GetDevice() { return _device; }
	ComPtr<IDXGIFactory4> GetFactory() { return _factory; }
	ComPtr<ID3D12CommandQueue> GetQueue() { return _commandQueue; }
	ComPtr<ID3D12CommandAllocator> GetAllocator() { return _commandAllocator; }
	ComPtr<ID3D12GraphicsCommandList> GetList() { return _commandList; }

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
	D3D12_VIEWPORT _viewport;
	D3D12_RECT _rect;

private:
	const float CLEAR_COLOR[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
};
