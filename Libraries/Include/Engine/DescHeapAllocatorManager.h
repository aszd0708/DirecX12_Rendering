#pragma once

struct DescriptorHandle;
class DescriptorHeapAllocator;

// 일단 이 매니저의 Allocator의 ID들은 D3D12_DESCRIPTOR_HEAP_TYPE 이 값들을 갖고와서 만듦.
class DescHeapAllocatorManager
{
	DECLARE_SINGLE(DescHeapAllocatorManager);

private:
	const UINT8 DEFAULT_HEAP_COUNT = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES + 1;

public:
	void Init();
	void Release();
	void ResetIndex(D3D12_DESCRIPTOR_HEAP_TYPE type);

public:
	ID3D12DescriptorHeap* GetDescriptorHeapAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type);
	DescriptorHandle AllocDescHandle(D3D12_DESCRIPTOR_HEAP_TYPE type);
	void FreeAllocDescHandle(const DescriptorHandle& handle);

private:
	DescriptorHeapAllocator** _allocators;
};

