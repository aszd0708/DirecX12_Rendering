#pragma once
#include "ShaderInfo.h"
#include "TextureInfo.h"
#include "MaterialInfo.h"

class Shader;
class Texture;

class Material : public IMemoryBlockHanlde
{
public:
	Material(const MaterialInfo& info, GpuCommandInfo* commandInfo);
	virtual ~Material() override;

private:
	void Init(const MaterialInfo& info, GpuCommandInfo* commandInfo);
	void SetTextures(const MaterialInfo& info);

public:
	Shader* GetShader() { return _shader; };
	const UINT32* GetTextureIndexes(OUT UINT8& count) const;
	const UINT64 GetMaterialInfoHash() { return _infoHash; }
	const UINT64 GetSlotIndex();

private:
	void GetMaterialDesc(const MaterialInfo& info, OUT void* data, OUT UINT64& size);

private:
	UINT64 _infoHash;
	Shader* _shader;
	
	Texture* _textures[MAX_TEXTURE_COUNT];
	UINT32 _textureIndexes[MAX_TEXTURE_COUNT];
	UINT8 _textureCount = 0;

	GpuMaterialBufferHandle _slot;
};

