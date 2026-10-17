#pragma once
#include "TextureInfo.h"

class GpuCommandInfo;

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
	void CreateTexture(GpuCommandInfo* commandInfo, OUT GpuMemoryHandle& uploadHandle);

private:
	void CreateResource(GpuCommandInfo* commandInfo, OUT GpuMemoryHandle& uploadHandle);
	void CreateView();

public:
	D3D12_GPU_DESCRIPTOR_HANDLE GetHandle() { return _descHandle.gpuDesc; }
	const TextureInfo& GetTextureInfo() { return _textureInfo; }
	const DescriptorHandle& GetDescHandle() { return _descHandle; }

private:
	TextureInfo _textureInfo;

	ComPtr<ID3D12Resource> _resource;
	D3D12_SHADER_RESOURCE_VIEW_DESC _resourceDesc;

	D3D12_DESCRIPTOR_HEAP_DESC _heapDesc;
	DescriptorHandle _descHandle;

	GpuMemoryHandle _memoryHandle;
};