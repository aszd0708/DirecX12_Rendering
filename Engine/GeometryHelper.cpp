#include "pch.h"
#include "GeometryHelper.h"
#include "VertexData.h"

Geometry GeometryHelper::CreateQuadVertexColorData(Color color)
{
	Geometry geometry = {};

	geometry.desces = VertexColorData::GetDesc();

	UINT32 vertexTotalSize = sizeof(VertexColorData) * 4;
	geometry.vertices = Array<BYTE>(vertexTotalSize);
	VertexColorData* vtx = (VertexColorData*)malloc(vertexTotalSize);
	vtx[0].position = Vec3(-0.5f, -0.5f, 0.f);
	vtx[0].color = color;
	vtx[1].position = Vec3(-0.5f, 0.5f, 0.f);
	vtx[1].color = color;
	vtx[2].position = Vec3(0.5f, -0.5f, 0.f);
	vtx[2].color = color;
	vtx[3].position = Vec3(0.5f, 0.5f, 0.f);
	vtx[3].color = color;
	memcpy(geometry.vertices.GetData(), vtx, vertexTotalSize);
	geometry.vertexSize = sizeof(VertexColorData);
	free(vtx);

	UINT32 indexTotalSize = sizeof(UINT16) * 6;
	geometry.indices = Array<BYTE>(indexTotalSize);
	UINT16* idx = (UINT16*)malloc(indexTotalSize);
	idx[0] = 0;
	idx[1] = 1;
	idx[2] = 2;
	idx[3] = 2;
	idx[4] = 1;
	idx[5] = 3;
	memcpy(geometry.indices.GetData(), idx, indexTotalSize);
	geometry.indexSize = eIndexBufferSize::UINT_16;
	free(idx);

	return geometry;
}

Geometry GeometryHelper::CreateCubeVertexColorData(Color color)
{
	Geometry geometry = {};

	geometry.desces = VertexColorData::GetDesc();

	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	UINT32 vertexTotalSize = sizeof(VertexColorData) * 24;
	geometry.vertices = Array<BYTE>(vertexTotalSize);
	VertexColorData* vtx = (VertexColorData*)malloc(vertexTotalSize);

	// 앞면
	vtx[0] = VertexColorData{ Vec4(-w2, -h2, -d2, 1.0f), color };
	vtx[1] = VertexColorData{ Vec4(-w2, +h2, -d2, 1.0f), color };
	vtx[2] = VertexColorData{ Vec4(+w2, +h2, -d2, 1.0f), color };
	vtx[3] = VertexColorData{ Vec4(+w2, -h2, -d2, 1.0f), color };
	// 뒷면
	vtx[4] = VertexColorData{ Vec4(-w2, -h2, +d2, 1.0f), color };
	vtx[5] = VertexColorData{ Vec4(+w2, -h2, +d2, 1.0f), color };
	vtx[6] = VertexColorData{ Vec4(+w2, +h2, +d2, 1.0f), color };
	vtx[7] = VertexColorData{ Vec4(-w2, +h2, +d2, 1.0f), color };
	// 윗면
	vtx[8] = VertexColorData{ Vec4(-w2, +h2, -d2, 1.0f), color };
	vtx[9] = VertexColorData{ Vec4(-w2, +h2, +d2, 1.0f), color };
	vtx[10] = VertexColorData{ Vec4(+w2, +h2, +d2, 1.0f), color };
	vtx[11] = VertexColorData{ Vec4(+w2, +h2, -d2, 1.0f), color };
	// 아랫면
	vtx[12] = VertexColorData{ Vec4(-w2, -h2, -d2, 1.0f), color };
	vtx[13] = VertexColorData{ Vec4(+w2, -h2, -d2, 1.0f), color };
	vtx[14] = VertexColorData{ Vec4(+w2, -h2, +d2, 1.0f), color };
	vtx[15] = VertexColorData{ Vec4(-w2, -h2, +d2, 1.0f), color };
	// 왼쪽면
	vtx[16] = VertexColorData{ Vec4(-w2, -h2, +d2, 1.0f), color };
	vtx[17] = VertexColorData{ Vec4(-w2, +h2, +d2, 1.0f), color };
	vtx[18] = VertexColorData{ Vec4(-w2, +h2, -d2, 1.0f), color };
	vtx[19] = VertexColorData{ Vec4(-w2, -h2, -d2, 1.0f), color };
	// 오른쪽면
	vtx[20] = VertexColorData{ Vec4(+w2, -h2, -d2, 1.0f), color };
	vtx[21] = VertexColorData{ Vec4(+w2, +h2, -d2, 1.0f), color };
	vtx[22] = VertexColorData{ Vec4(+w2, +h2, +d2, 1.0f), color };
	vtx[23] = VertexColorData{ Vec4(+w2, -h2, +d2, 1.0f), color };

	memcpy(geometry.vertices.GetData(), vtx, vertexTotalSize);
	geometry.vertexSize = sizeof(VertexColorData);
	free(vtx);

	UINT32 indexTotalSize = sizeof(UINT16) * 36;
	geometry.indices = Array<BYTE>(indexTotalSize);
	UINT16* idx = (UINT16*)malloc(indexTotalSize);

	// 앞면
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// 뒷면
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// 윗면
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// 아랫면
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// 왼쪽면
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// 오른쪽면
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	memcpy(geometry.indices.GetData(), idx, indexTotalSize);
	geometry.indexSize = eIndexBufferSize::UINT_16;
	free(idx);

	return geometry;
}

Geometry GeometryHelper::CreateSphereVertexColorData(Color color)
{
	Geometry geometry = {};

	geometry.desces = VertexColorData::GetDesc();

	float radius = 0.5f; // 구의 반지름
	uint32 stackCount = 20; // 가로 분할
	uint32 sliceCount = 20; // 세로 분할

	UINT32 vertexTotalSize = sizeof(VertexColorData) * ((stackCount - 1) * (sliceCount + 1) + 2);
	geometry.vertices = Array<BYTE>(vertexTotalSize);
	VertexColorData* vtx = (VertexColorData*)malloc(vertexTotalSize);
	UINT32 index = 0;


	// 북극
	VertexColorData north;
	north.position = Vec4(0.0f, radius, 0.0f, 1.0f);
	north.color = color;
	vtx[index] = north;
	index++;

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	// 고리마다 돌면서 정점을 계산한다 (북극/남극 단일점은 고리가 X)
	for (uint32 y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// 고리에 위치한 정점
		for (uint32 x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			VertexColorData v;

			v.position.x = radius * sinf(phi) * cosf(theta);
			v.position.y = radius * cosf(phi);
			v.position.z = radius * sinf(phi) * sinf(theta);
			v.position.w = 1.0f;

			v.color = color;

			vtx[index] = v;
			index++;
		}
	}

	// 남극
	VertexColorData south;
	south.position = Vec4(0.0f, -radius, 0.0f, 1.0f);
	south.color = color;
	vtx[index] = south;

	memcpy(geometry.vertices.GetData(), vtx, vertexTotalSize);
	geometry.vertexSize = sizeof(VertexColorData);
	free(vtx);


	UINT32 indexTotalSize = sizeof(UINT16) * ((sliceCount + 1) * 3 + (stackCount - 2) * sliceCount * 6 + (sliceCount * 3));
	geometry.indices = Array<BYTE>(indexTotalSize);
	UINT16* idx = (UINT16*)malloc(indexTotalSize);
	index = 0;
	// 북극 인덱스
	for (uint32 i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
		//  [i+1]-[i+2]
		idx[index++] = (0);
		idx[index++] = (i + 2);
		idx[index++] = (i + 1);
	}

	// 몸통 인덱스
	uint32 ringVertexCount = sliceCount + 1;
	for (uint32 y = 0; y < stackCount - 2; ++y)
	{
		for (uint32 x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx[index++] = (1 + (y)*ringVertexCount + (x));
			idx[index++] = (1 + (y)*ringVertexCount + (x + 1));
			idx[index++] = (1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx[index++] =(1 + (y + 1) * ringVertexCount + (x));
			idx[index++] =(1 + (y)*ringVertexCount + (x + 1));
			idx[index++] =(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// 남극 인덱스
	uint32 bottomIndex = static_cast<uint32>(index - 1);
	uint32 lastRingStartIndex = bottomIndex - ringVertexCount;
	for (uint32 i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx[index++] =(bottomIndex);
		idx[index++] =(lastRingStartIndex + i);
		idx[index++] =(lastRingStartIndex + i + 1);
	}


	memcpy(geometry.indices.GetData(), idx, indexTotalSize);
	geometry.indexSize = eIndexBufferSize::UINT_16;
	free(idx);

	return geometry;
}

Geometry GeometryHelper::CreateGridVertexColorData(int32 sizeX, int32 sizeZ, Color color)
{
	Geometry geometry = {};

	geometry.desces = VertexColorData::GetDesc();

	UINT32 vertexTotalSize = sizeof(VertexColorData) * ((sizeZ + 1) * (sizeX + 1));
	geometry.vertices = Array<BYTE>(vertexTotalSize);
	VertexColorData* vtx = (VertexColorData*)malloc(vertexTotalSize);
	UINT32 index = 0;

	for (int32 z = 0; z < sizeZ + 1; z++)
	{
		for (int32 x = 0; x < sizeX + 1; x++)
		{
			VertexColorData v;
			v.position = Vec4(static_cast<float>(x), 0, static_cast<float>(z), 1.0f);
			v.color = color;

			vtx[index++] = v;
		}
	}

	memcpy(geometry.vertices.GetData(), vtx, vertexTotalSize);
	geometry.vertexSize = sizeof(VertexColorData);
	free(vtx);

	UINT32 indexTotalSize = sizeof(UINT16) * (sizeX * sizeZ * 6);
	geometry.indices = Array<BYTE>(indexTotalSize);
	UINT16* idx = (UINT16*)malloc(indexTotalSize);
	index = 0;

	for (int32 z = 0; z < sizeZ; z++)
	{
		for (int32 x = 0; x < sizeX; x++)
		{
			//  [0]
			//   |	\
			//  [2] - [1]
			idx[index++] = ((sizeX + 1) * (z + 1) + (x));
			idx[index++] = ((sizeX + 1) * (z)+(x + 1));
			idx[index++] = ((sizeX + 1) * (z)+(x));
			//  [1] - [2]
			//   	\  |
			//		  [0]
			idx[index++] = ((sizeX + 1) * (z)+(x + 1));
			idx[index++] = ((sizeX + 1) * (z + 1) + (x));
			idx[index++] = ((sizeX + 1) * (z + 1) + (x + 1));
		}
	}

	memcpy(geometry.indices.GetData(), idx, indexTotalSize);
	geometry.indexSize = eIndexBufferSize::UINT_16;
	free(idx);

	return geometry;
}

Geometry GeometryHelper::CreateQuadVertexTextureData()
{
	Geometry geometry = {};

	geometry.desces = VertexTextureData::GetDesc();

	UINT32 vertexTotalSize = sizeof(VertexTextureData) * 4;
	geometry.vertices = Array<BYTE>(vertexTotalSize);
	VertexTextureData* vtx = (VertexTextureData*)malloc(vertexTotalSize);
	vtx[0].position = Vec3(-0.5f, -0.5f, 0.f);
	vtx[0].uv = Vec2(0.f, 1.f);
	vtx[1].position = Vec3(-0.5f, 0.5f, 0.f);
	vtx[1].uv = Vec2(0.f, 0.f);
	vtx[2].position = Vec3(0.5f, -0.5f, 0.f);
	vtx[2].uv = Vec2(1.f, 1.f);
	vtx[3].position = Vec3(0.5f, 0.5f, 0.f);
	vtx[3].uv = Vec2(1.f, 0.f);
	memcpy(geometry.vertices.GetData(), vtx, vertexTotalSize);
	geometry.vertexSize = sizeof(VertexTextureData);
	free(vtx);

	UINT32 indexTotalSize = sizeof(UINT16) * 6;
	geometry.indices = Array<BYTE>(indexTotalSize);
	UINT16* idx = (UINT16*)malloc(indexTotalSize);
	idx[0] = 0;
	idx[1] = 1;
	idx[2] = 2;
	idx[3] = 2;
	idx[4] = 1;
	idx[5] = 3;
	memcpy(geometry.indices.GetData(), idx, indexTotalSize);
	geometry.indexSize = eIndexBufferSize::UINT_16;
	free(idx);

	return geometry;
}

Geometry GeometryHelper::CreateCubeVertexTextureData()
{
	Geometry geometry = {};

	geometry.desces = VertexTextureData::GetDesc();

	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	UINT32 vertexTotalSize = sizeof(VertexTextureData) * 24;
	geometry.vertices = Array<BYTE>(vertexTotalSize);
	VertexTextureData* vtx = (VertexTextureData*)malloc(vertexTotalSize);

	// 앞면
	vtx[0] = VertexTextureData{ Vec3(-w2, -h2, -d2), Vec2(0.0f, 1.0f) };
	vtx[1] = VertexTextureData{ Vec3(-w2, +h2, -d2), Vec2(0.0f, 0.0f) };
	vtx[2] = VertexTextureData{ Vec3(+w2, +h2, -d2), Vec2(1.0f, 0.0f) };
	vtx[3] = VertexTextureData{ Vec3(+w2, -h2, -d2), Vec2(1.0f, 1.0f) };
	// 뒷면
	vtx[4] = VertexTextureData{ Vec3(-w2, -h2, +d2), Vec2(1.0f, 1.0f) };
	vtx[5] = VertexTextureData{ Vec3(+w2, -h2, +d2), Vec2(0.0f, 1.0f) };
	vtx[6] = VertexTextureData{ Vec3(+w2, +h2, +d2), Vec2(0.0f, 0.0f) };
	vtx[7] = VertexTextureData{ Vec3(-w2, +h2, +d2), Vec2(1.0f, 0.0f) };
	// 윗면
	vtx[8] = VertexTextureData{ Vec3(-w2, +h2, -d2), Vec2(0.0f, 1.0f) };
	vtx[9] = VertexTextureData{ Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f) };
	vtx[10] = VertexTextureData{ Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f) };
	vtx[11] = VertexTextureData{ Vec3(+w2, +h2, -d2), Vec2(1.0f, 1.0f) };
	// 아랫면
	vtx[12] = VertexTextureData{ Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f) };
	vtx[13] = VertexTextureData{ Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f) };
	vtx[14] = VertexTextureData{ Vec3(+w2, -h2, +d2), Vec2(0.0f, 0.0f) };
	vtx[15] = VertexTextureData{ Vec3(-w2, -h2, +d2), Vec2(1.0f, 0.0f) };
	// 왼쪽면
	vtx[16] = VertexTextureData{ Vec3(-w2, -h2, +d2), Vec2(0.0f, 1.0f) };
	vtx[17] = VertexTextureData{ Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f) };
	vtx[18] = VertexTextureData{ Vec3(-w2, +h2, -d2), Vec2(1.0f, 0.0f) };
	vtx[19] = VertexTextureData{ Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f) };
	// 오른쪽면
	vtx[20] = VertexTextureData{ Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f) };
	vtx[21] = VertexTextureData{ Vec3(+w2, +h2, -d2), Vec2(0.0f, 0.0f) };
	vtx[22] = VertexTextureData{ Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f) };
	vtx[23] = VertexTextureData{ Vec3(+w2, -h2, +d2), Vec2(1.0f, 1.0f) };

	memcpy(geometry.vertices.GetData(), vtx, vertexTotalSize);
	geometry.vertexSize = sizeof(VertexTextureData);
	free(vtx);

	UINT32 indexTotalSize = sizeof(UINT16) * 36;
	geometry.indices = Array<BYTE>(indexTotalSize);
	UINT16* idx = (UINT16*)malloc(indexTotalSize);

	// 앞면
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// 뒷면
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// 윗면
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// 아랫면
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// 왼쪽면
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// 오른쪽면
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	memcpy(geometry.indices.GetData(), idx, indexTotalSize);
	geometry.indexSize = eIndexBufferSize::UINT_16;
	free(idx);

	return geometry;
}

Geometry GeometryHelper::CreateSphereVertexTextureData()
{
	Geometry geometry = {};

	geometry.desces = VertexTextureData::GetDesc();

	float radius = 0.5f; // 구의 반지름
	uint32 stackCount = 20; // 가로 분할
	uint32 sliceCount = 20; // 세로 분할

	UINT32 vertexTotalSize = sizeof(VertexTextureData) * ((stackCount - 1) * (sliceCount + 1) + 2);
	geometry.vertices = Array<BYTE>(vertexTotalSize);
	VertexTextureData* vtx = (VertexTextureData*)malloc(vertexTotalSize);
	UINT32 index = 0;

	// 북극
	VertexTextureData north;
	north.position = Vec3(0.0f, radius, 0.0f);
	north.uv = Vec2(0.5f, 0.0f);
	vtx[index] = north;
	index++;

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	// 고리마다 돌면서 정점을 계산한다 (북극/남극 단일점은 고리가 X)
	for (uint32 y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// 고리에 위치한 정점
		for (uint32 x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			VertexTextureData v;

			v.position.x = radius * sinf(phi) * cosf(theta);
			v.position.y = radius * cosf(phi);
			v.position.z = radius * sinf(phi) * sinf(theta);

			v.uv = Vec2(deltaU * x, deltaV * y);

			vtx[index] = v;
			index++;
		}
	}

	// 남극
	VertexTextureData south;
	south.position = Vec3(0.0f, -radius, 0.0f);
	south.uv = Vec2(0.5f, 1.0f);
	vtx[index] = south;

	memcpy(geometry.vertices.GetData(), vtx, vertexTotalSize);
	geometry.vertexSize = sizeof(VertexTextureData);
	free(vtx);


	UINT32 indexTotalSize = sizeof(UINT16) * ((sliceCount + 1) * 3 + (stackCount - 2) * sliceCount * 6 + (sliceCount * 3));
	geometry.indices = Array<BYTE>(indexTotalSize);
	UINT16* idx = (UINT16*)malloc(indexTotalSize);
	index = 0;
	// 북극 인덱스
	for (uint32 i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
		//  [i+1]-[i+2]
		idx[index++] = (0);
		idx[index++] = (i + 2);
		idx[index++] = (i + 1);
	}

	// 몸통 인덱스
	uint32 ringVertexCount = sliceCount + 1;
	for (uint32 y = 0; y < stackCount - 2; ++y)
	{
		for (uint32 x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx[index++] = (1 + (y)*ringVertexCount + (x));
			idx[index++] = (1 + (y)*ringVertexCount + (x + 1));
			idx[index++] = (1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx[index++] = (1 + (y + 1) * ringVertexCount + (x));
			idx[index++] = (1 + (y)*ringVertexCount + (x + 1));
			idx[index++] = (1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// 남극 인덱스
	uint32 bottomIndex = static_cast<uint32>(index - 1);
	uint32 lastRingStartIndex = bottomIndex - ringVertexCount;
	for (uint32 i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx[index++] = (bottomIndex);
		idx[index++] = (lastRingStartIndex + i);
		idx[index++] = (lastRingStartIndex + i + 1);
	}


	memcpy(geometry.indices.GetData(), idx, indexTotalSize);
	geometry.indexSize = eIndexBufferSize::UINT_16;
	free(idx);

	return geometry;
}

Geometry GeometryHelper::CreateGridVertexTextureData(INT32 sizeX, INT32 sizeZ)
{
	Geometry geometry = {};

	geometry.desces = VertexTextureData::GetDesc();

	UINT32 vertexTotalSize = sizeof(VertexTextureData) * ((sizeZ + 1) * (sizeX + 1));
	geometry.vertices = Array<BYTE>(vertexTotalSize);
	VertexTextureData* vtx = (VertexTextureData*)malloc(vertexTotalSize);
	UINT32 index = 0;

	for (int32 z = 0; z < sizeZ + 1; z++)
	{
		for (int32 x = 0; x < sizeX + 1; x++)
		{
			VertexTextureData v;
			v.position = Vec3(static_cast<float>(x), 0, static_cast<float>(z));
			v.uv = Vec2(static_cast<float>(x), static_cast<float>(z));

			vtx[index++] = v;
		}
	}

	memcpy(geometry.vertices.GetData(), vtx, vertexTotalSize);
	geometry.vertexSize = sizeof(VertexTextureData);
	free(vtx);

	UINT32 indexTotalSize = sizeof(UINT16) * (sizeX * sizeZ * 6);
	geometry.indices = Array<BYTE>(indexTotalSize);
	UINT16* idx = (UINT16*)malloc(indexTotalSize);
	index = 0;

	for (int32 z = 0; z < sizeZ; z++)
	{
		for (int32 x = 0; x < sizeX; x++)
		{
			//  [0]
			//   |	\
			//  [2] - [1]
			idx[index++] = ((sizeX + 1) * (z + 1) + (x));
			idx[index++] = ((sizeX + 1) * (z)+(x + 1));
			idx[index++] = ((sizeX + 1) * (z)+(x));
			//  [1] - [2]
			//   	\  |
			//		  [0]
			idx[index++] = ((sizeX + 1) * (z)+(x + 1));
			idx[index++] = ((sizeX + 1) * (z + 1) + (x));
			idx[index++] = ((sizeX + 1) * (z + 1) + (x + 1));
		}
	}

	memcpy(geometry.indices.GetData(), idx, indexTotalSize);
	geometry.indexSize = eIndexBufferSize::UINT_16;
	free(idx);

	return geometry;
}

Geometry GeometryHelper::CreateQuadVertexTextureNormalData()
{
	Geometry geometry = {};

	//geometry.descs = VertexTextureNormalData::GetDesc();

	UINT32 vertexTotalSize = sizeof(VertexTextureNormalData) * 4;
	geometry.vertices = Array<BYTE>(vertexTotalSize);
	VertexTextureNormalData* vtx = (VertexTextureNormalData*)malloc(vertexTotalSize);
	vtx[0].position = Vec3(-0.5f, -0.5f, 0.f);
	vtx[0].uv = Vec2(0.f, 1.f);
	vtx[0].normal = Vec3(0.f, 0.f, -1.f);
	vtx[1].position = Vec3(-0.5f, 0.5f, 0.f);
	vtx[1].uv = Vec2(0.f, 0.f);
	vtx[1].normal = Vec3(0.f, 0.f, -1.f);
	vtx[2].position = Vec3(0.5f, -0.5f, 0.f);
	vtx[2].uv = Vec2(1.f, 1.f);
	vtx[2].normal = Vec3(0.f, 0.f, -1.f);
	vtx[3].position = Vec3(0.5f, 0.5f, 0.f);
	vtx[3].uv = Vec2(1.f, 0.f);
	vtx[3].normal = Vec3(0.f, 0.f, -1.f);
	memcpy(geometry.vertices.GetData(), vtx, vertexTotalSize);
	geometry.vertexSize = sizeof(VertexTextureNormalData);
	free(vtx);

	UINT32 indexTotalSize = sizeof(UINT16) * 6;
	geometry.indices = Array<BYTE>(indexTotalSize);
	UINT16* idx = (UINT16*)malloc(indexTotalSize);
	idx[0] = 0;
	idx[1] = 1;
	idx[2] = 2;
	idx[3] = 2;
	idx[4] = 1;
	idx[5] = 3;
	memcpy(geometry.indices.GetData(), idx, indexTotalSize);
	geometry.indexSize = eIndexBufferSize::UINT_16;
	free(idx);

	return geometry;
}

Geometry GeometryHelper::CreateCubeVertexTextureNormalData()
{
	Geometry geometry = {};

	//geometry.descs = VertexTextureNormalData::GetDesc();

	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	UINT32 vertexTotalSize = sizeof(VertexTextureNormalData) * 24;
	geometry.vertices = Array<BYTE>(vertexTotalSize);
	VertexTextureNormalData* vtx = (VertexTextureNormalData*)malloc(vertexTotalSize);

	// 앞면
	vtx[0] = VertexTextureNormalData(Vec3(-w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f));
	vtx[1] = VertexTextureNormalData(Vec3(-w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vtx[2] = VertexTextureNormalData(Vec3(+w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vtx[3] = VertexTextureNormalData(Vec3(+w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f));
	// 뒷면
	vtx[4] = VertexTextureNormalData(Vec3(-w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f));
	vtx[5] = VertexTextureNormalData(Vec3(+w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f));
	vtx[6] = VertexTextureNormalData(Vec3(+w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vtx[7] = VertexTextureNormalData(Vec3(-w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	// 윗면
	vtx[8] = VertexTextureNormalData(Vec3(-w2, +h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f));
	vtx[9] = VertexTextureNormalData(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	vtx[10] = VertexTextureNormalData(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	vtx[11] = VertexTextureNormalData(Vec3(+w2, +h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f));
	// 아랫면
	vtx[12] = VertexTextureNormalData(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f));
	vtx[13] = VertexTextureNormalData(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f));
	vtx[14] = VertexTextureNormalData(Vec3(+w2, -h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f));
	vtx[15] = VertexTextureNormalData(Vec3(-w2, -h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f));
	// 왼쪽면
	vtx[16] = VertexTextureNormalData(Vec3(-w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vtx[17] = VertexTextureNormalData(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vtx[18] = VertexTextureNormalData(Vec3(-w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vtx[19] = VertexTextureNormalData(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	// 오른쪽면
	vtx[20] = VertexTextureNormalData(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vtx[21] = VertexTextureNormalData(Vec3(+w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vtx[22] = VertexTextureNormalData(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vtx[23] = VertexTextureNormalData(Vec3(+w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f));

	memcpy(geometry.vertices.GetData(), vtx, vertexTotalSize);
	geometry.vertexSize = sizeof(VertexTextureNormalData);
	free(vtx);

	UINT32 indexTotalSize = sizeof(UINT16) * 36;
	geometry.indices = Array<BYTE>(indexTotalSize);
	UINT16* idx = (UINT16*)malloc(indexTotalSize);

	// 앞면
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// 뒷면
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// 윗면
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// 아랫면
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// 왼쪽면
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// 오른쪽면
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	memcpy(geometry.indices.GetData(), idx, indexTotalSize);
	geometry.indexSize = eIndexBufferSize::UINT_16;
	free(idx);

	return geometry;
}

Geometry GeometryHelper::CreateGridVertexTextureNormalData(INT32 sizeX, INT32 sizeZ)
{
	Geometry geometry = {};

	//geometry.descs = VertexTextureNormalData::GetDesc();

	UINT32 vertexTotalSize = sizeof(VertexTextureNormalData) * ((sizeZ + 1) * (sizeX + 1));
	geometry.vertices = Array<BYTE>(vertexTotalSize);
	VertexTextureNormalData* vtx = (VertexTextureNormalData*)malloc(vertexTotalSize);
	UINT32 index = 0;

	for (int32 z = 0; z < sizeZ + 1; z++)
	{
		for (int32 x = 0; x < sizeX + 1; x++)
		{
			VertexTextureNormalData v;
			v.position = Vec3(static_cast<float>(x), 0, static_cast<float>(z));
			v.uv = Vec2(static_cast<float>(x), static_cast<float>(sizeZ - z));
			v.normal = Vec3(0.f, 1.f, 0.f);

			vtx[index++] = v;
		}
	}

	memcpy(geometry.vertices.GetData(), vtx, vertexTotalSize);
	geometry.vertexSize = sizeof(VertexTextureNormalData);
	free(vtx);

	UINT32 indexTotalSize = sizeof(UINT16) * (sizeX * sizeZ * 6);
	geometry.indices = Array<BYTE>(indexTotalSize);
	UINT16* idx = (UINT16*)malloc(indexTotalSize);
	index = 0;

	for (int32 z = 0; z < sizeZ; z++)
	{
		for (int32 x = 0; x < sizeX; x++)
		{
			//  [0]
			//   |	\
			//  [2] - [1]
			idx[index++] = ((sizeX + 1) * (z + 1) + (x));
			idx[index++] = ((sizeX + 1) * (z)+(x + 1));
			idx[index++] = ((sizeX + 1) * (z)+(x));
			//  [1] - [2]
			//   	\  |
			//		  [0]
			idx[index++] = ((sizeX + 1) * (z)+(x + 1));
			idx[index++] = ((sizeX + 1) * (z + 1) + (x));
			idx[index++] = ((sizeX + 1) * (z + 1) + (x + 1));
		}
	}

	memcpy(geometry.indices.GetData(), idx, indexTotalSize);
	geometry.indexSize = eIndexBufferSize::UINT_16;
	free(idx);

	return geometry;
}

Geometry GeometryHelper::CreateSphereVertexTextureNormalData()
{
	Geometry geometry = {};

	//geometry.descs = VertexTextureNormalData::GetDesc();

	float radius = 0.5f; // 구의 반지름
	uint32 stackCount = 20; // 가로 분할
	uint32 sliceCount = 20; // 세로 분할

	UINT32 vertexTotalSize = sizeof(VertexTextureNormalData) * ((stackCount - 1) * (sliceCount + 1) + 2);
	geometry.vertices = Array<BYTE>(vertexTotalSize);
	VertexTextureNormalData* vtx = (VertexTextureNormalData*)malloc(vertexTotalSize);
	UINT32 index = 0;

	// 북극
	VertexTextureNormalData north;
	north.position = Vec3(0.0f, radius, 0.0f);
	north.uv = Vec2(0.5f, 0.0f);
	north.normal = north.position;
	north.normal.Normalize();
	vtx[index] = north;
	index++;

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	// 고리마다 돌면서 정점을 계산한다 (북극/남극 단일점은 고리가 X)
	for (uint32 y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// 고리에 위치한 정점
		for (uint32 x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			VertexTextureNormalData v;

			v.position.x = radius * sinf(phi) * cosf(theta);
			v.position.y = radius * cosf(phi);
			v.position.z = radius * sinf(phi) * sinf(theta);

			v.uv = Vec2(deltaU * x, deltaV * y);

			v.normal = v.position;
			v.normal.Normalize();

			vtx[index] = v;
			index++;
		}
	}

	// 남극
	VertexTextureNormalData south;
	south.position = Vec3(0.0f, -radius, 0.0f);
	south.uv = Vec2(0.5f, 1.0f);
	south.normal = south.position;
	south.normal.Normalize();
	vtx[index] = south;

	memcpy(geometry.vertices.GetData(), vtx, vertexTotalSize);
	geometry.vertexSize = sizeof(VertexTextureNormalData);
	free(vtx);


	UINT32 indexTotalSize = sizeof(UINT16) * ((sliceCount + 1) * 3 + (stackCount - 2) * sliceCount * 6 + (sliceCount * 3));
	geometry.indices = Array<BYTE>(indexTotalSize);
	UINT16* idx = (UINT16*)malloc(indexTotalSize);
	index = 0;
	// 북극 인덱스
	for (uint32 i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
		//  [i+1]-[i+2]
		idx[index++] = (0);
		idx[index++] = (i + 2);
		idx[index++] = (i + 1);
	}

	// 몸통 인덱스
	uint32 ringVertexCount = sliceCount + 1;
	for (uint32 y = 0; y < stackCount - 2; ++y)
	{
		for (uint32 x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx[index++] = (1 + (y)*ringVertexCount + (x));
			idx[index++] = (1 + (y)*ringVertexCount + (x + 1));
			idx[index++] = (1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx[index++] = (1 + (y + 1) * ringVertexCount + (x));
			idx[index++] = (1 + (y)*ringVertexCount + (x + 1));
			idx[index++] = (1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// 남극 인덱스
	uint32 bottomIndex = static_cast<uint32>(index - 1);
	uint32 lastRingStartIndex = bottomIndex - ringVertexCount;
	for (uint32 i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx[index++] = (bottomIndex);
		idx[index++] = (lastRingStartIndex + i);
		idx[index++] = (lastRingStartIndex + i + 1);
	}


	memcpy(geometry.indices.GetData(), idx, indexTotalSize);
	geometry.indexSize = eIndexBufferSize::UINT_16;
	free(idx);

	return geometry;
}

Geometry GeometryHelper::CreateQuadVertexTextureNormalTangentData()
{
	Geometry geometry = {};

	//geometry.descs = VertexTextureNormalTangentData::GetDesc();

	UINT32 vertexTotalSize = sizeof(VertexTextureNormalTangentData) * 4;
	geometry.vertices = Array<BYTE>(vertexTotalSize);
	VertexTextureNormalTangentData* vtx = (VertexTextureNormalTangentData*)malloc(vertexTotalSize);
	vtx[0].position = Vec3(-0.5f, -0.5f, 0.f);
	vtx[0].uv = Vec2(0.f, 1.f);
	vtx[0].normal = Vec3(0.f, 0.f, -1.f);
	vtx[0].tangent = Vec3(1.0f, 0.0f, 0.0f);
	vtx[1].position = Vec3(-0.5f, 0.5f, 0.f);
	vtx[1].uv = Vec2(0.f, 0.f);
	vtx[1].normal = Vec3(0.f, 0.f, -1.f);
	vtx[1].tangent = Vec3(1.0f, 0.0f, 0.0f);
	vtx[2].position = Vec3(0.5f, -0.5f, 0.f);
	vtx[2].uv = Vec2(1.f, 1.f);
	vtx[2].normal = Vec3(0.f, 0.f, -1.f);
	vtx[2].tangent = Vec3(1.0f, 0.0f, 0.0f);
	vtx[3].position = Vec3(0.5f, 0.5f, 0.f);
	vtx[3].uv = Vec2(1.f, 0.f);
	vtx[3].normal = Vec3(0.f, 0.f, -1.f);
	vtx[3].tangent = Vec3(1.0f, 0.0f, 0.0f);
	memcpy(geometry.vertices.GetData(), vtx, vertexTotalSize);
	geometry.vertexSize = sizeof(VertexTextureNormalTangentData);
	free(vtx);

	UINT32 indexTotalSize = sizeof(UINT16) * 6;
	geometry.indices = Array<BYTE>(indexTotalSize);
	UINT16* idx = (UINT16*)malloc(indexTotalSize);
	idx[0] = 0;
	idx[1] = 1;
	idx[2] = 2;
	idx[3] = 2;
	idx[4] = 1;
	idx[5] = 3;
	memcpy(geometry.indices.GetData(), idx, indexTotalSize);
	geometry.indexSize = eIndexBufferSize::UINT_16;
	free(idx);

	return geometry;
}

Geometry GeometryHelper::CreateCubeVertexTextureNormalTangentData()
{
	Geometry geometry = {};

	//geometry.descs = VertexTextureNormalTangentData::GetDesc();

	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	UINT32 vertexTotalSize = sizeof(VertexTextureNormalTangentData) * 24;
	geometry.vertices = Array<BYTE>(vertexTotalSize);
	VertexTextureNormalTangentData* vtx = (VertexTextureNormalTangentData*)malloc(vertexTotalSize);

	// 앞면
	vtx[0] = VertexTextureNormalTangentData(Vec3(-w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vtx[1] = VertexTextureNormalTangentData(Vec3(-w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vtx[2] = VertexTextureNormalTangentData(Vec3(+w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vtx[3] = VertexTextureNormalTangentData(Vec3(+w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	// 뒷면
	vtx[4] = VertexTextureNormalTangentData(Vec3(-w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vtx[5] = VertexTextureNormalTangentData(Vec3(+w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vtx[6] = VertexTextureNormalTangentData(Vec3(+w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vtx[7] = VertexTextureNormalTangentData(Vec3(-w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	// 윗면
	vtx[8] = VertexTextureNormalTangentData(Vec3(-w2, +h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vtx[9] = VertexTextureNormalTangentData(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vtx[10] = VertexTextureNormalTangentData(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vtx[11] = VertexTextureNormalTangentData(Vec3(+w2, +h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	// 아랫면
	vtx[12] = VertexTextureNormalTangentData(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vtx[13] = VertexTextureNormalTangentData(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vtx[14] = VertexTextureNormalTangentData(Vec3(+w2, -h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vtx[15] = VertexTextureNormalTangentData(Vec3(-w2, -h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	// 왼쪽면
	vtx[16] = VertexTextureNormalTangentData(Vec3(-w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vtx[17] = VertexTextureNormalTangentData(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vtx[18] = VertexTextureNormalTangentData(Vec3(-w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vtx[19] = VertexTextureNormalTangentData(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	// 오른쪽면
	vtx[20] = VertexTextureNormalTangentData(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vtx[21] = VertexTextureNormalTangentData(Vec3(+w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vtx[22] = VertexTextureNormalTangentData(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vtx[23] = VertexTextureNormalTangentData(Vec3(+w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));

	memcpy(geometry.vertices.GetData(), vtx, vertexTotalSize);
	geometry.vertexSize = sizeof(VertexTextureNormalTangentData);
	free(vtx);

	UINT32 indexTotalSize = sizeof(UINT16) * 36;
	geometry.indices = Array<BYTE>(indexTotalSize);
	UINT16* idx = (UINT16*)malloc(indexTotalSize);

	// 앞면
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// 뒷면
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// 윗면
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// 아랫면
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// 왼쪽면
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// 오른쪽면
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	memcpy(geometry.indices.GetData(), idx, indexTotalSize);
	geometry.indexSize = eIndexBufferSize::UINT_16;
	free(idx);

	return geometry;
}

Geometry GeometryHelper::CreateGridVertexTextureNormalTangentData(int32 sizeX, int32 sizeZ)
{
	Geometry geometry = {};

	//geometry.descs = VertexTextureNormalTangentData::GetDesc();

	UINT32 vertexTotalSize = sizeof(VertexTextureNormalTangentData) * ((sizeZ + 1) * (sizeX + 1));
	geometry.vertices = Array<BYTE>(vertexTotalSize);
	VertexTextureNormalTangentData* vtx = (VertexTextureNormalTangentData*)malloc(vertexTotalSize);
	UINT32 index = 0;

	for (int32 z = 0; z < sizeZ + 1; z++)
	{
		for (int32 x = 0; x < sizeX + 1; x++)
		{
			VertexTextureNormalTangentData v;
			v.position = Vec3(static_cast<float>(x), 0, static_cast<float>(z));
			v.uv = Vec2(static_cast<float>(x), static_cast<float>(sizeZ - z));
			v.normal = Vec3(0.f, 1.f, 0.f);
			v.tangent = Vec3(1.f, 0.f, 0.f);

			vtx[index++] = v;
		}
	}

	memcpy(geometry.vertices.GetData(), vtx, vertexTotalSize);
	geometry.vertexSize = sizeof(VertexTextureNormalTangentData);
	free(vtx);

	UINT32 indexTotalSize = sizeof(UINT16) * (sizeX * sizeZ * 6);
	geometry.indices = Array<BYTE>(indexTotalSize);
	UINT16* idx = (UINT16*)malloc(indexTotalSize);
	index = 0;

	for (int32 z = 0; z < sizeZ; z++)
	{
		for (int32 x = 0; x < sizeX; x++)
		{
			//  [0]
			//   |	\
			//  [2] - [1]
			idx[index++] = ((sizeX + 1) * (z + 1) + (x));
			idx[index++] = ((sizeX + 1) * (z)+(x + 1));
			idx[index++] = ((sizeX + 1) * (z)+(x));
			//  [1] - [2]
			//   	\  |
			//		  [0]
			idx[index++] = ((sizeX + 1) * (z)+(x + 1));
			idx[index++] = ((sizeX + 1) * (z + 1) + (x));
			idx[index++] = ((sizeX + 1) * (z + 1) + (x + 1));
		}
	}

	memcpy(geometry.indices.GetData(), idx, indexTotalSize);
	geometry.indexSize = eIndexBufferSize::UINT_16;
	free(idx);

	return geometry;
}

Geometry GeometryHelper::CreateSphereVertexTextureNormalTangentData()
{
	Geometry geometry = {};

	//geometry.descs = VertexTextureNormalTangentData::GetDesc();

	float radius = 0.5f; // 구의 반지름
	uint32 stackCount = 20; // 가로 분할
	uint32 sliceCount = 20; // 세로 분할

	UINT32 vertexTotalSize = sizeof(VertexTextureNormalTangentData) * ((stackCount - 1) * (sliceCount + 1) + 2);
	geometry.vertices = Array<BYTE>(vertexTotalSize);
	VertexTextureNormalTangentData* vtx = (VertexTextureNormalTangentData*)malloc(vertexTotalSize);
	UINT32 index = 0;

	// 북극
	VertexTextureNormalTangentData north;
	north.position = Vec3(0.0f, radius, 0.0f);
	north.uv = Vec2(0.5f, 0.0f);
	north.normal = north.position;
	north.normal.Normalize();
	north.tangent = Vec3(1.0f, 0.0f, 0.0f);
	north.tangent.Normalize();
	vtx[index] = north;
	index++;

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	// 고리마다 돌면서 정점을 계산한다 (북극/남극 단일점은 고리가 X)
	for (uint32 y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// 고리에 위치한 정점
		for (uint32 x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			VertexTextureNormalTangentData v;

			v.position.x = radius * sinf(phi) * cosf(theta);
			v.position.y = radius * cosf(phi);
			v.position.z = radius * sinf(phi) * sinf(theta);

			v.uv = Vec2(deltaU * x, deltaV * y);

			v.normal = v.position;
			v.normal.Normalize();

			v.tangent.x = -radius * sinf(phi) * sinf(theta);
			v.tangent.y = 0.0f;
			v.tangent.z = radius * sinf(phi) * cosf(theta);
			v.tangent.Normalize();

			vtx[index] = v;
			index++;
		}
	}

	// 남극
	VertexTextureNormalTangentData south;
	south.position = Vec3(0.0f, -radius, 0.0f);
	south.uv = Vec2(0.5f, 1.0f);
	south.normal = south.position;
	south.normal.Normalize();
	south.tangent = Vec3(1.0f, 0.0f, 0.0f);
	south.tangent.Normalize();
	vtx[index] = south;

	memcpy(geometry.vertices.GetData(), vtx, vertexTotalSize);
	geometry.vertexSize = sizeof(VertexTextureNormalTangentData);
	free(vtx);


	UINT32 indexTotalSize = sizeof(UINT16) * ((sliceCount + 1) * 3 + (stackCount - 2) * sliceCount * 6 + (sliceCount * 3));
	geometry.indices = Array<BYTE>(indexTotalSize);
	UINT16* idx = (UINT16*)malloc(indexTotalSize);
	index = 0;
	// 북극 인덱스
	for (uint32 i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
		//  [i+1]-[i+2]
		idx[index++] = (0);
		idx[index++] = (i + 2);
		idx[index++] = (i + 1);
	}

	// 몸통 인덱스
	uint32 ringVertexCount = sliceCount + 1;
	for (uint32 y = 0; y < stackCount - 2; ++y)
	{
		for (uint32 x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx[index++] = (1 + (y)*ringVertexCount + (x));
			idx[index++] = (1 + (y)*ringVertexCount + (x + 1));
			idx[index++] = (1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx[index++] = (1 + (y + 1) * ringVertexCount + (x));
			idx[index++] = (1 + (y)*ringVertexCount + (x + 1));
			idx[index++] = (1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// 남극 인덱스
	uint32 bottomIndex = static_cast<uint32>(index - 1);
	uint32 lastRingStartIndex = bottomIndex - ringVertexCount;
	for (uint32 i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx[index++] = (bottomIndex);
		idx[index++] = (lastRingStartIndex + i);
		idx[index++] = (lastRingStartIndex + i + 1);
	}


	memcpy(geometry.indices.GetData(), idx, indexTotalSize);
	geometry.indexSize = eIndexBufferSize::UINT_16;
	free(idx);

	return geometry;
}