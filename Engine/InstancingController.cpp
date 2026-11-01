#include "pch.h"
#include "InstancingController.h"
#include "GpuConstantBufferPoolManager.h"
#include "GpuCommandPool.h"
#include "ConstantBuffer.h"
#include "Shader.h"
#include "Mesh.h"

bool InstancingController::TURN_ON_INSTANCING = true;
UINT32 InstancingController::DRAW_CALL_COUNT = 0;

InstanceGroup::InstanceGroup(Mesh* mesh, Shader* shader) : _mesh(mesh), _shader(shader), _instanceDesces()
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
	for (int i = 0; i < _instanceArray.GetCount(); ++i)
	{
		delete _instanceArray[i];
	}
}

void InstancingController::Add(const InstancingInfo& info)
{
	UINT64 key = GetKey(info.mesh->GetInstancingID(), info.shader->GetInstancingID());
	
	InstanceGroup* group = nullptr;
	bool isFound = _instanceTable.GetValue(key, group);
	if (isFound)
	{
		if (group->GetDescCount() <= 0)
		{
			group->SetMesh(info.mesh);
			group->SetShader(info.shader);
		}
		group->AddInstanceDesc(info.instanceDesc);
	}
	else
	{
		group = new InstanceGroup(info.mesh, info.shader);
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

void InstancingController::Render(const ComPtr<ID3D12GraphicsCommandList>& commandList)
{
	DRAW_CALL_COUNT = 0;
	for (int i = 0; i < _instanceArray.GetCount(); ++i)
	{
		InstanceGroup* group = _instanceArray[i];
		UINT32 descCount = group->GetDescCount();
		if (descCount <= 0) continue;

		UINT64 bufferSize = sizeof(InstanceDesc) * descCount;
		// 여기 인스턴싱 하는 부분
		ConstantBuffer buffer(bufferSize);
		D3D12_GPU_VIRTUAL_ADDRESS address = {};
		buffer.PushDataWithGetAddress(group->GetDesces(), bufferSize, address);

		Shader* shader = group->GetShader();
		Mesh* mesh = group->GetMesh();

		commandList->SetPipelineState(shader->GetPSO().Get());
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Vertex 버퍼 전달
		D3D12_VERTEX_BUFFER_VIEW vertexView = mesh->GetVertexView();
		commandList->IASetVertexBuffers(0, 1, &vertexView);

		// Index 버퍼 전달
		D3D12_INDEX_BUFFER_VIEW indexView = mesh->GetIndexView();
		commandList->IASetIndexBuffer(&indexView);

		// 인스턴싱 세팅
		if (TURN_ON_INSTANCING)
		{
			commandList->SetGraphicsRootShaderResourceView((UINT)eShaderIndex::INSTANCE, address);
			commandList->DrawIndexedInstanced(mesh->GetIndexCount(), descCount, 0, 0, 0);
			DRAW_CALL_COUNT++;
		}
		else
		{
			for (UINT32 j = 0; j < descCount; ++j)
			{
				commandList->SetGraphicsRootShaderResourceView((UINT)eShaderIndex::INSTANCE, address + j * sizeof(InstanceDesc));
				commandList->DrawIndexedInstanced(mesh->GetIndexCount(), 1, 0, 0, 0);
				DRAW_CALL_COUNT++;
			}
		}
	}
}

UINT64 InstancingController::GetKey(UINT32 meshID, UINT32 shaderID)
{
	UINT64 key = ((UINT64)shaderID << 32) | meshID;

	return key;
}