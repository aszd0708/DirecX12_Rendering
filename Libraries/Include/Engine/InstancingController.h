#pragma once

class Mesh;
class Shader;

struct InstancingInfo
{
	Mesh* mesh;
	Shader* shader;
	InstanceDesc instanceDesc;
};

class InstanceGroup
{
public:
	InstanceGroup(Mesh* mesh, Shader* shader);
	~InstanceGroup();

	void AddInstanceDesc(const InstanceDesc& instanceDesc);

	void SetMesh(Mesh* mesh) { _mesh = mesh; }
	Mesh* GetMesh() { return _mesh; }

	void SetShader(Shader* shader) {_shader = shader;}
	Shader* GetShader() { return _shader; }

	const InstanceDesc* GetDesces();
	UINT32 GetDescCount(){ return _instanceDesces.GetCount(); }

	void Clear();

private:
	Mesh* _mesh;
	Shader* _shader;
	DynamicArray<InstanceDesc> _instanceDesces;
};

/// <summary>
/// Scene에 하나씩 대응
/// </summary>
class InstancingController
{
public:
	static bool TURN_ON_INSTANCING;
	static UINT32 DRAW_CALL_COUNT;

public:
	InstancingController();
	~InstancingController();

public:
	void Add(const InstancingInfo& info);
	void Clear();
	void Render(const ComPtr<ID3D12GraphicsCommandList>& commandList);

private:
	UINT64 GetKey(UINT32 meshID, UINT32 shaderID);

private:
	HashTable<UINT64, InstanceGroup*> _instanceTable;
	DynamicArray<InstanceGroup*> _instanceArray;
};

