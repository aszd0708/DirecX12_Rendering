#pragma once
#include "ShaderInfo.h"
#include "TextureInfo.h"
#include "MaterialInfo.h"

class Shader;
class Texture;

class Material : public IMemoryBlockHanlde
{
public:
	Material(const MaterialInfo& info);
	virtual ~Material() override;

private:
	void Init(const MaterialInfo& info);
	void SetTextures(const MaterialInfo& info);

public:
	Shader* GetShader() { return _shader; };
	const UINT32* GetTextureIndexes(OUT UINT8& count) const;

private:
	Shader* _shader;
	
	Texture* _textures[MAX_TEXTURE_COUNT];
	UINT32 _textureIndexes[MAX_TEXTURE_COUNT];
	UINT8 _textureCount = 0;
};

