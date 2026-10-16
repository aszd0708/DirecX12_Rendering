#pragma once

// 최대 텍스처 갯수
constexpr UINT8 MAX_TEXTURE_COUNT = 8;

enum class eMaterialType : UINT8
{
	ERROR_TYPE = 0,

	DEFAULT,
	PBR,
	SKY_MAP,
	TERRAIN,

	MAX
};

enum class eDefaultTextureType : UINT8
{
	NORMAL = 0,
	DIFFUSE,

	MAX
};