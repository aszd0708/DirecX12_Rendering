#include "pch.h"
#include "DescHeapAllocatorManager.h"
#include "DescriptorHeapAllocator.h"

void DescHeapAllocatorManager::Init()
{
	_allocators = (DescriptorHeapAllocator**)malloc(sizeof(DescriptorHeapAllocator*) * (DEFAULT_HEAP_COUNT));

	for (int i = 0; i < DEFAULT_HEAP_COUNT; ++i)
	{
		_allocators[i] = nullptr;
	}
}

void DescHeapAllocatorManager::Release()
{
	for (int i = 0; i < DEFAULT_HEAP_COUNT; ++i)
	{
		if (_allocators[i] != nullptr)
		{
			delete _allocators[i];
		}
	}

	free(_allocators);
}

void DescHeapAllocatorManager::ResetIndex(D3D12_DESCRIPTOR_HEAP_TYPE type)
{
	DescriptorHeapAllocator* allocator = _allocators[(UINT8)type];
	if (allocator != nullptr)
	{
		allocator->ResetAllocator();
	}
}

void DescHeapAllocatorManager::FreeAllocDescHandle(const DescriptorHandle& handle)
{
	DescriptorHeapAllocator* allocator = _allocators[handle.allocatorID];
	allocator->Free(handle);
}

ID3D12DescriptorHeap* DescHeapAllocatorManager::GetDescriptorHeapAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type)
{
	assert(_allocators[(UINT8)type] != nullptr);

	DescriptorHeapAllocator* allocator = _allocators[(UINT8)type];
	if (allocator != nullptr)
	{
		DescriptorHeapPage* page = allocator->GetHeapPage();
		if (page != nullptr)
		{
			return page->GetDescHeap();
		}
	}
	return nullptr;
}

DescriptorHandle DescHeapAllocatorManager::AllocDescHandle(D3D12_DESCRIPTOR_HEAP_TYPE type)
{
	UINT8 index = (UINT8)type;
	if (_allocators[index] == nullptr)
	{
		_allocators[index] = new DescriptorHeapAllocator(index, type);
	}

	return _allocators[index]->Alloc();
}
