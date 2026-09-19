#pragma once

class Geometry
{
public:
	Geometry(Array<D3D12_INPUT_ELEMENT_DESC> desces, UINT8 vertexSize, Array<BYTE> vertices, UINT8 indexSize, Array<BYTE> indices);
	~Geometry();

	const Array<D3D12_INPUT_ELEMENT_DESC>& GetVertexDesc() { return _desces; }

	UINT32 GetVertexCount() { return _vertices.GetCount() / _vertexSize; }
	void* GetVertexData() { return _vertices.GetData(); }
	UINT32 GetVertexSize() { return _vertexSize; };

	UINT32 GetIndexCount() { return _indices.GetCount() / _indexSize;  }
	void* GetIndexData() { return _indices.GetData(); }
	UINT32 GetIndexSize() { return _indexSize; }

private:
	UINT8 _vertexSize;
	UINT8 _indexSize;

	Array<BYTE> _vertices;
	Array<BYTE> _indices;

	Array<D3D12_INPUT_ELEMENT_DESC> _desces;
};