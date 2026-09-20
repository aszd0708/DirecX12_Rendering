#pragma once
#include "TextureInfo.h"

struct TextureFormat
{
	wstring filePath;
	int x;
	int y;
	int channelsInFile;
};

class Texture : public IMemoryBlockHanlde
{

public:
	Texture(TextureInfo textureInfo);
	Texture(wstring filePath, DXGI_FORMAT format, UINT16 mipLevels);
	virtual ~Texture();

public:
	void CreateTexture(const ComPtr<ID3D12GraphicsCommandList>& commandList, OUT GpuMemoryHandle& uploadHandle);

private:
	void CreateResource(const ComPtr<ID3D12GraphicsCommandList>& commandList, OUT GpuMemoryHandle& uploadHandle);
	void CreateView();

public:
	D3D12_GPU_DESCRIPTOR_HANDLE GetHandle() { return _descHandle.gpuDesc; }
	const TextureInfo& GetTextureInfo() { return _textureInfo; }

private:
	TextureInfo _textureInfo;

	ComPtr<ID3D12Resource> _resource;
	D3D12_SHADER_RESOURCE_VIEW_DESC _resourceDesc;

	D3D12_DESCRIPTOR_HEAP_DESC _heapDesc;
	DescriptorHandle _descHandle;

	GpuMemoryHandle _memoryHandle;
};