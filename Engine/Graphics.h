#pragma once

class Graphics
{
	DECLARE_SINGLE(Graphics);

public:
	static const DXGI_FORMAT DEPTH_STENCIL_FORMAT = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	static constexpr FLOAT DEPTH_CLEAR_VALUE = 1.0f;
	static const UINT8 STENCIL_CLEAR_VALUE = 0;

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
	void CreateBackBufferDSV();
	void CreateFence();

	void CreateRootSignature();

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

	ComPtr<ID3D12CommandAllocator> GetSubAllocator() { return _commandAllocatorSub; }
	ComPtr<ID3D12GraphicsCommandList> GetSubList() { return _commandListSub; }

	ComPtr<ID3D12RootSignature> GetRootSignature() { return _signature; }

	UINT64 GetNextFenceValue() { return _fenceValue + 1; }
	UINT64 GetCompletedFenceValue() { return _fence->GetCompletedValue(); }

private:
	HWND _hwnd = {};

	ComPtr<ID3D12Device> _device;
	ComPtr<IDXGIFactory4> _factory;
	ComPtr<ID3D12CommandQueue> _commandQueue;
	ComPtr<ID3D12CommandAllocator> _commandAllocator;
	ComPtr<ID3D12GraphicsCommandList> _commandList;

	ComPtr<ID3D12CommandAllocator> _commandAllocatorSub;
	ComPtr<ID3D12GraphicsCommandList> _commandListSub;

	ComPtr<IDXGISwapChain3> _swapChain;
	ComPtr<ID3D12Resource> _renderTargets[SWAP_CHAIN_BUFFER_COUNT];

	ComPtr<ID3D12DescriptorHeap> _rtvHeap;
	UINT64 _rtvDescriptorSize = 0;
	ComPtr<ID3D12DescriptorHeap> _dsvHeap;
	UINT64 _dsvDescriptorSize = 0;
	ComPtr<ID3D12Resource> _dsvTarget;

	ComPtr<ID3D12Fence> _fence;
	UINT64 _fenceValue = 0;
	HANDLE _fenceEvent = nullptr;

	// Root Signature
	ComPtr<ID3D12RootSignature> _signature;

private:
	D3D12_VIEWPORT _viewport;
	D3D12_RECT _rect;

private:
	const float CLEAR_COLOR[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
};
