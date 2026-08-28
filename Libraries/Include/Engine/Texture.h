#pragma once

struct TextureFormat
{
	wstring filePath;
	int x;
	int y;
	int channelsInFile;
};

class Texture
{

public:
	Texture(wstring filePath);
	~Texture();

public:
	void CreateTexture();
	void CreateResource();
	void CreateView();

public:
	ComPtr<ID3D12DescriptorHeap> GetDescHeap() { return _descHeap; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetHandle() { return _descHeap->GetGPUDescriptorHandleForHeapStart(); }

private:
	wstring _filePath;

	ComPtr<ID3D12Resource> _resource;
	D3D12_SHADER_RESOURCE_VIEW_DESC _resourceDesc;

	ComPtr<ID3D12DescriptorHeap> _descHeap;
	D3D12_DESCRIPTOR_HEAP_DESC _heapDesc;
};

