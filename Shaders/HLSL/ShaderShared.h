#ifndef SHADER_SHARED_H
#define SHADER_SHARED_H

#define CONCAT_IMPL(a, b)   a ## b
#define CONCAT(a, b)        CONCAT_IMPL(a, b)

// Constant Buffer (b)
#define GLOBAL_REGISTER					0
#define TRANSFORM_REGISTER				1
#define TEXTURE_INDEX_REGISTER			2

// Sampler (s)
#define LINEAR_SAMPLER_REGISTER			0

// Instancing 
#define INSTANCE_REGISTER				0	

// Material
#define MATERIAL_REGISTER       		1	

// 최대 텍스처 갯수
#define MAX_TEXTURE_COUNT				8

// For Default MATERIAL Texture Type
#define DEFAULT_MATERIAL_TEX_NORMAL		0
#define DEFAULT_MATERIAL_TEX_DIFFUSE	1
#define DEFAULT_MATERIAL_TEX_COUNT		2

// For PBR MATERIAL Texture Type
#define PBR_MATERIAL_TEX_BASE_COLOR		0
#define PBR_MATERIAL_TEX_EMISSIVE		1
#define PBR_MATERIAL_TEX_METALLIC		2
#define PBR_MATERIAL_TEX_ROUGHNESS		3
#define PBR_MATERIAL_TEX_AO				4
#define PBR_MATERIAL_TEX_NORMAL			5
#define PBR_MATERAIL_TEX_COUNT			6

// Animation
#define ANIMATION_REGISTER				2

#endif