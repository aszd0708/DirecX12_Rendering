#pragma once

struct VertexTextureData
{
	Vec3 position = { 0, 0, 0 };
	Vec2 uv = { 0, 0 };

	static inline vector<D3D12_INPUT_ELEMENT_DESC> GetDesc()
	{
		vector<D3D12_INPUT_ELEMENT_DESC> desc;
		// Position
		{
			D3D12_INPUT_ELEMENT_DESC inputElementDesc = {};
			inputElementDesc.SemanticName = "POSITION";
			inputElementDesc.SemanticIndex = 0;
			inputElementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			inputElementDesc.InputSlot = 0;
			inputElementDesc.AlignedByteOffset = 0;
			inputElementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			inputElementDesc.InstanceDataStepRate = 0;
			desc.emplace_back(inputElementDesc);
		}

		// Color
		{
			D3D12_INPUT_ELEMENT_DESC inputElementDesc = {};
			inputElementDesc.SemanticName = "TEXCOORD";
			inputElementDesc.SemanticIndex = 0;
			inputElementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			inputElementDesc.InputSlot = 0;
			inputElementDesc.AlignedByteOffset = sizeof(Vec3);
			inputElementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			inputElementDesc.InstanceDataStepRate = 0;
			desc.emplace_back(inputElementDesc);
		}
		return desc;
	}
};

struct VertexColorData
{
	Vec4 position = { 0, 0, 0, 0 };
	Color color = { 0, 0, 0, 0 };

	static inline vector<D3D12_INPUT_ELEMENT_DESC> GetDesc()
	{
		vector<D3D12_INPUT_ELEMENT_DESC> desc;
		// Position
		{
			D3D12_INPUT_ELEMENT_DESC inputElementDesc = {};
			inputElementDesc.SemanticName = "POSITION";
			inputElementDesc.SemanticIndex = 0;
			inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			inputElementDesc.InputSlot = 0;
			inputElementDesc.AlignedByteOffset = 0;
			inputElementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			inputElementDesc.InstanceDataStepRate = 0;
			desc.emplace_back(inputElementDesc);
		}

		// Color
		{
			D3D12_INPUT_ELEMENT_DESC inputElementDesc = {};
			inputElementDesc.SemanticName = "COLOR";
			inputElementDesc.SemanticIndex = 0;
			inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			inputElementDesc.InputSlot = 0;
			inputElementDesc.AlignedByteOffset = sizeof(Vec4);
			inputElementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			inputElementDesc.InstanceDataStepRate = 0;
			desc.emplace_back(inputElementDesc);
		}
		return desc;
	}
};

struct VertexTextureNormalData
{
	Vec3 position = { 0, 0, 0 };
	Vec2 uv = { 0, 0 };
	Vec3 normal = { 0, 0, 0 };
};

struct VertexTextureNormalTangentData
{
	Vec3 position = { 0, 0, 0 };
	Vec2 uv = { 0, 0 };
	Vec3 normal = { 0, 0, 0 };
	Vec3 tangent = { 0, 0, 0 };
};

struct VertexTextureNormalTangentBlendData
{
	Vec3 position = { 0, 0, 0 };
	Vec2 uv = { 0, 0 };
	Vec3 normal = { 0, 0, 0 };
	Vec3 tangent = { 0, 0, 0 };
	Vec4 blendIndices = { 0, 0, 0, 0};
	Vec4 blendWeights = { 0, 0, 0, 0};
};

using ModelVertexType = VertexTextureNormalTangentBlendData;