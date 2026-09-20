#include "pch.h"
#include "GpuCommandPool.h"

GpuCommandInfo::GpuCommandInfo(UINT8 id, ComPtr<ID3D12GraphicsCommandList> commandList, ComPtr<ID3D12CommandAllocator> commandAllocator) :
	_poolID(id), _commandAddedCount(0), _commandList(commandList), _allocator(commandAllocator)
{

}

GpuCommandInfo::~GpuCommandInfo()
{
	_commandList.Reset();
	_allocator.Reset();
}

void GpuCommandInfo::AddUploadHandle(const GpuMemoryHandle& handle)
{
	_uploadHandles.Add(handle);
}

void GpuCommandInfo::Reset()
{
	_allocator->Reset();
	_commandList->Reset(_allocator.Get(), nullptr);
	_commandAddedCount = 0;
}

void GpuCommandInfo::Close()
{
	_commandList->Close();
	_commandAddedCount = 0;
}

const UINT8& GpuCommandInfo::GetPoolID()
{
	return _poolID;
}

void GpuCommandInfo::SetPoolID(const UINT8 id)
{
	_poolID = id;
}

void GpuCommandInfo::AddCommandCount()
{
	_commandAddedCount++;
}

const UINT16& GpuCommandInfo::GetCommandAddedCount()
{
	return _commandAddedCount;
}

const ComPtr<ID3D12GraphicsCommandList>& GpuCommandInfo::GetCommandList()
{
	return _commandList;
}

const ComPtr<ID3D12CommandAllocator>& GpuCommandInfo::GetCommandAllocator()
{
	return _allocator;
}


void GpuCommandPoolManager::Init(UINT8 poolCount)
{
	_pool = Array<GpuCommandInfo*>(poolCount);

	for (int i = 0; i < poolCount; ++i)
	{
		GpuCommandInfo* pool = CreatePool(i);
		_pool[i] = pool;
		_poolIndexStack.Push(i);
	}
}

void GpuCommandPoolManager::Release()
{
	for (int i = 0; i < _pool.GetCount(); ++i)
	{
		delete _pool[i];
	}
}

GpuCommandInfo* GpuCommandPoolManager::CreatePool(UINT8 id)
{
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ThrowIfFailed(DEVICE->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)));
	ThrowIfFailed(DEVICE->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));

	GpuCommandInfo* pool = new GpuCommandInfo(id, commandList, commandAllocator);
	return pool;
}

bool GpuCommandPoolManager::GetCommandPool(OUT GpuCommandInfo** pool)
{
	if (_poolIndexStack.GetCount() <= 0)
	{
		return false;
	}

	UINT8 index = 255;
	bool isSuccess = _poolIndexStack.Pop(index);
	assert(isSuccess);

	*pool = _pool[index];
	return isSuccess;
}

bool GpuCommandPoolManager::ReleaseCommandPool(GpuCommandInfo* pool)
{
	UINT8 index = pool->GetPoolID();
	_poolIndexStack.Push(index);
	
	return true;
}

void GpuCommandPoolManager::AddSendingQueueIndex(UINT8 index)
{
	_sendingQueue.Add(index);
}

void GpuCommandPoolManager::SendQueue()
{
	UINT8 count = _sendingQueue.GetCount();
	assert(count > 0);
	ID3D12CommandList** list = (ID3D12CommandList**)malloc(sizeof(ID3D12CommandList*) * count);
	for (int i = 0; i < count; ++i)
	{
		UINT8 index = _sendingQueue[i];
		ComPtr<ID3D12GraphicsCommandList> commandList = _pool[index]->GetCommandList();
		list[i] = commandList.Get();
	}

	COMMAND_QUEUE->ExecuteCommandLists(count, list);

	GRAPHICS->WaitForGPU();

	_sendingQueue.Clear();

	free(list);
}
