#include "pch.h"
#include "GpuConstantBufferPool.h"
#include "GpuConstantBufferPoolManager.h"

void GpuConstantBufferPoolManager::Init(UINT8 pageCount, UINT64 pageSize)
{
    _constantBufferPool = new GpuConstantBufferPool(pageSize, pageCount);
}

void GpuConstantBufferPoolManager::Release()
{
    delete _constantBufferPool;
}

bool GpuConstantBufferPoolManager::GetMemory(UINT64 size, OUT GpuConstantBufferHandle& handle)
{
    return _constantBufferPool->GetBufferHandle(size, handle);
}

const ComPtr<ID3D12Resource>& GpuConstantBufferPoolManager::GetMemoryResource()
{
    return _constantBufferPool->GetResource();
}

void* GpuConstantBufferPoolManager::GetMappedBase()
{
    return _constantBufferPool->GetMapedBase();
}

void GpuConstantBufferPoolManager::ResetCurrentPage()
{
    _constantBufferPool->ResetCurrentPage();
}

void GpuConstantBufferPoolManager::ResetAllPage()
{
    _constantBufferPool->ResetAllPage();
}

void GpuConstantBufferPoolManager::MoveNextPage(ID3D12Fence* fence, UINT64 fenceValue)
{
    _constantBufferPool->MoveNextPage(fence, fenceValue);
}
