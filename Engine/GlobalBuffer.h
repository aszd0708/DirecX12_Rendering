#pragma once

struct GlobalDesc
{
	Matrix V = Matrix::Identity;
	Matrix P = Matrix::Identity;
	Matrix VP = Matrix::Identity;
	Matrix VInv = Matrix::Identity;

	static inline vector<D3D12_INPUT_ELEMENT_DESC> GetDesc()
	{
		vector<D3D12_INPUT_ELEMENT_DESC> desc;
		// Position
		{
			D3D12_INPUT_ELEMENT_DESC inputElementDesc = {};
			inputElementDesc.SemanticName = "V";
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
			inputElementDesc.SemanticName = "UV";
			inputElementDesc.SemanticIndex = 0;
			inputElementDesc.Format = DXGI_FORMAT_R16G16_FLOAT;
			inputElementDesc.InputSlot = 0;
			inputElementDesc.AlignedByteOffset = sizeof(Matrix);
			inputElementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			inputElementDesc.InstanceDataStepRate = 0;
			desc.emplace_back(inputElementDesc);
		}
		return desc;
	}
};

struct ScreenSizeDesc
{
	int width;
	int height;

	Vec2 padding;
};

struct TransformDesc
{
	Matrix W = Matrix::Identity;
};
