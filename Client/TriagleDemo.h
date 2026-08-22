#pragma once
#include "SceneBuilder.h"

class TriagleDemo : public SceneBuilder
{
	struct VertexData
	{
		Vec3 _vertex;
		Vec4 _color;

		static int GetSize()
		{
			int vertexSize = sizeof(Vec3);
			int colorSize =  sizeof(Vec4);
			return vertexSize + colorSize;
		}
	};

public:
	TriagleDemo(string sceneName);
	virtual ~TriagleDemo();

public:
	virtual void Init() override;
	virtual void Render() override;

// For Demo
private: // Render

public:

private:
	void CreateTriangle();
	void CreateResource();
	void CreateRootSignature();
	void CreatePSO();

private:
	vector<VertexData> _vertexData;

	ComPtr<ID3DBlob> _vsBlob;
	ComPtr<ID3DBlob> _psBlob;

	ComPtr<ID3DBlob> _vsError;
	ComPtr<ID3DBlob> _psError;

	ComPtr<ID3D12Resource> _resource;
	D3D12_VERTEX_BUFFER_VIEW _resourceView;

	ComPtr<ID3DBlob> _signatureBlob;
	ComPtr<ID3DBlob> _signatureError;
	ComPtr<ID3D12RootSignature> _signature;
	
	ComPtr<ID3D12PipelineState> _pso;
};

