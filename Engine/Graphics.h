#pragma once

class Graphics
{
	DECLARE_SINGLE(Graphics);

public:
	void Init(HWND hwnd);

	void RenderBegin();
	void RenderEnd();

private:
	void CreateCommandQueue();
	void CreateCommandAllocator();
	void CreateCommandList();

	void CreateFactory();
	void CreateSwapChain();

private:
	HWND _hwnd = {};

	ComPtr<ID3D12Device> _device;
	ComPtr<IDXGIFactory4> _factory;
	ComPtr<ID3D12CommandQueue> _commandQueue;
	ComPtr<ID3D12CommandAllocator> _commandAllocator;
	ComPtr<ID3D12CommandList> _commandList;

	ComPtr<IDXGISwapChain3> _swapChain;

private:
	const int SwapChainCount = 2;
};
