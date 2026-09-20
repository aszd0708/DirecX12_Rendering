#pragma once
class GpuCommandInfo
{
public:
	GpuCommandInfo(UINT8 id, ComPtr<ID3D12GraphicsCommandList> commandList, ComPtr<ID3D12CommandAllocator> commandAllocator);
	~GpuCommandInfo();

public:
	void AddUploadHandle(const GpuMemoryHandle& handle);
	void Reset();
	void Close();

public:
	const UINT8& GetPoolID();
	void SetPoolID(const UINT8 id);
	void AddCommandCount();
	const UINT16& GetCommandAddedCount();
	const ComPtr<ID3D12GraphicsCommandList>& GetCommandList();
	const ComPtr<ID3D12CommandAllocator>& GetCommandAllocator();

private:
	UINT8 _poolID;
	UINT16 _commandAddedCount;
	ComPtr<ID3D12GraphicsCommandList> _commandList;
	ComPtr<ID3D12CommandAllocator> _allocator;

	DynamicArray<GpuMemoryHandle> _uploadHandles;
};

class GpuCommandPoolManager
{
	DECLARE_SINGLE(GpuCommandPoolManager);

public:
	void Init(UINT8 poolCount);
	void Release();

private:
	GpuCommandInfo* CreatePool(UINT8 id);

public:
	bool GetCommandPool(OUT GpuCommandInfo** pool);
	bool ReleaseCommandPool(GpuCommandInfo* pool);

	void AddSendingQueueIndex(UINT8 index);
	void SendQueue();

private:
	Stack<UINT8> _poolIndexStack;
	Array<GpuCommandInfo*> _pool;

	DynamicArray<UINT8> _sendingQueue;
};

