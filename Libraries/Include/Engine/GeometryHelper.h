#pragma once
#include "GpuBufferInfo.h"

struct Geometry
{
	UINT8 vertexSize;
	eIndexBufferSize indexSize;

	Array<D3D12_INPUT_ELEMENT_DESC> desces;

	Array<BYTE> vertices;
	Array<BYTE> indices;
};

class GeometryHelper
{
public:
	static Geometry CreateQuadVertexColorData(Color color);
	static Geometry CreateCubeVertexColorData(Color color);
	static Geometry CreateSphereVertexColorData(Color color);
	static Geometry CreateGridVertexColorData(INT32 sizeX, INT32 sizeZ, Color color);
		   
	static Geometry CreateQuadVertexTextureData();
	static Geometry CreateCubeVertexTextureData();
	static Geometry CreateSphereVertexTextureData();
	static Geometry CreateGridVertexTextureData(INT32 sizeX, INT32 sizeZ);
		   
	static Geometry CreateQuadVertexTextureNormalData();
	static Geometry CreateCubeVertexTextureNormalData();
	static Geometry CreateSphereVertexTextureNormalData();
	static Geometry CreateGridVertexTextureNormalData(INT32 sizeX, INT32 sizeZ);
		   
	static Geometry CreateQuadVertexTextureNormalTangentData();
	static Geometry CreateCubeVertexTextureNormalTangentData();
	static Geometry CreateSphereVertexTextureNormalTangentData();
	static Geometry CreateGridVertexTextureNormalTangentData(INT32 sizeX, INT32 sizeZ);
};

