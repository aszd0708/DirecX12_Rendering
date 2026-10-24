#pragma once
#include "ModelData.h"

namespace tinyxml2
{
	class XMLElement;
}

/// <summary>
/// AssimpTool Converter가 만든 .mesh(바이너리) / .xml(Material) 을 읽어서 ModelData를 채움
/// CPU 데이터만 다루고 GPU 리소스는 만들지 않음
/// </summary>
class ModelLoader
{
public:
	/// <summary>
	/// 실패 시 false, 이때 modelData 내용은 사용하지 말 것
	/// shaderInfo : 파일에 셰이더 정보가 없으므로 호출하는 쪽이 정해서 모든 Material에 넣음
	/// </summary>
	static bool LoadModel(const wstring& meshPath, const wstring& materialPath, const ShaderInfo& shaderInfo, OUT ModelData& modelData);

private:
	static bool LoadMaterials(const wstring& materialPath, const ShaderInfo& shaderInfo, OUT Array<MaterialInfoVariant>& materials, OUT Array<string>& materialNames);
	static bool LoadMeshes(const wstring& meshPath, const Array<string>& materialNames, OUT ModelData& modelData);

	static void ReadTextureInfo(const tinyxml2::XMLElement* materialNode, const char* elementName, const char* legacyElementName, const wstring& folderPath, OUT TextureInfo& textureInfo);
	static INT32 FindMaterialIndex(const Array<string>& materialNames, const string& materialName);
};
