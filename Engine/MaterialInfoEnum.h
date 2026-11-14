#pragma once

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

enum class ePBRTextureType : UINT8
{
	BASE_COLOR = 0,
	EMISSIVE,
	METALIC,
	ROUGHNESS,
	AO,
	NORMAL,

	MAX
};