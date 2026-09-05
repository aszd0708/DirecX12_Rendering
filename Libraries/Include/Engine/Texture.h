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
	D3D12_GPU_DESCRIPTOR_HANDLE GetHandle() { return _descHandle.gpuDesc; }

private:
	wstring _filePath;

	ComPtr<ID3D12Resource> _resource;
	D3D12_SHADER_RESOURCE_VIEW_DESC _resourceDesc;

	D3D12_DESCRIPTOR_HEAP_DESC _heapDesc;
	DescriptorHandle _descHandle;
};

