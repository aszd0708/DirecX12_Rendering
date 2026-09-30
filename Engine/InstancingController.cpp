#include "pch.h"
#include "InstancingController.h"
#include "GpuConstantBufferPoolManager.h"
#include "GpuCommandPool.h"
#include "ConstantBuffer.h"

InstanceGroup::InstanceGroup(const MemoryBlock& memBlock) : _renderMemBlock(memBlock), _instanceDesces()
{

}

InstanceGroup::~InstanceGroup()
{

}

void InstanceGroup::AddInstanceDesc(const InstanceDesc& instanceDesc)
{
	_instanceDesces.Add(instanceDesc);
}

const InstanceDesc* InstanceGroup::GetDesces()
{
	if (_instanceDesces.GetCount() <= 0)
	{
		return nullptr;
	}
	return &_instanceDesces[0];
}

void InstanceGroup::Clear()
{
	_instanceDesces.Clear();
}

InstancingController::InstancingController() : _instanceTable(), _instanceArray()
{

}

InstancingController::~InstancingController()
{

}

void InstancingController::Add(const InstancingInfo& info)
{
	UINT64 key = GetKey(info.meshID, info.shaderID);
	
	InstanceGroup* group = nullptr;
	bool isFound = _instanceTable.GetValue(key, group);
	if (isFound)
	{
		if (group->GetDescCount() <= 0)
		{
			group->SetRenderMemoryBlock(info.renderMemBlock);
		}
		group->AddInstanceDesc(info.instanceDesc);
	}
	else
	{
		group = new InstanceGroup(info.renderMemBlock);
		group->AddInstanceDesc(info.instanceDesc);
		_instanceTable.Add(key, group);
		_instanceArray.Add(group);
	}
}

void InstancingController::Clear()
{
	for (int i = 0; i < _instanceArray.GetCount(); ++i)
	{
		_instanceArray[i]->Clear();
	}
}

void InstancingController::Render(GpuCommandInfo* commandInfo)
{
	for (int i = 0; i < _instanceArray.GetCount(); ++i)
	{
		UINT32 descCount = _instanceArray[i]->GetDescCount();
		if(descCount <= 0) continue;

		MemoryBlock memBlock = _instanceArray[i]->GetRenderMemoryBlock();
		CpuMemoryPool* pool = CPU_MEM_POOL->GetMemoryPool(memBlock._poolID);
		Renderer* renderer = nullptr;
		bool isSuccess = pool->GetObjectByMemoryBlock<Renderer>(memBlock, &renderer);
		if (isSuccess)
		{
			UINT64 bufferSize = sizeof(InstanceDesc) * descCount;
			// 여기 인스턴싱 하는 부분
			ConstantBuffer buffer(bufferSize);
			D3D12_GPU_VIRTUAL_ADDRESS address = {};
			buffer.PushDataWithGetAddress(_instanceArray[i]->GetDesces(), bufferSize, address);

			renderer->RenderInstancing(COMMAND_LIST.Get(), address, descCount);
		}
	}
}

UINT64 InstancingController::GetKey(UINT32 meshID, UINT32 shaderID)
{
	UINT64 key = ((UINT64)shaderID << 32) | meshID;

	return key;
}