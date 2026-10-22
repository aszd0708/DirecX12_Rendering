#include "pch.h"
#include "ModelLoader.h"
#include "FileUtils.h"
#include "Utils.h"
#include "VertexData.h"
#include "tinyxml2.h"
#include <filesystem>

static_assert(sizeof(ModelVertexType) <= UINT8_MAX, "Geometry::vertexSize가 UINT8");

bool ModelLoader::LoadModel(const wstring& meshPath, const wstring& materialPath, const ShaderInfo& shaderInfo, OUT ModelData& modelData)
{
	// Mesh가 materialName으로 Material을 찾기 때문에 Material 먼저 로드
	Array<string> materialNames;
	bool isSuccess = LoadMaterials(materialPath, shaderInfo, modelData.materials, materialNames);
	if (isSuccess == false) return false;

	isSuccess = LoadMeshes(meshPath, materialNames, modelData);
	if (isSuccess == false) return false;

	modelData.name = std::filesystem::path(meshPath).stem().string();

	return true;
}

bool ModelLoader::LoadMaterials(const wstring& materialPath, const ShaderInfo& shaderInfo, OUT Array<MaterialInfoVariant>& materials, OUT Array<string>& materialNames)
{
	if (std::filesystem::exists(materialPath) == false) return false;

	tinyxml2::XMLDocument document;
	tinyxml2::XMLError error = document.LoadFile(Utils::ToString(materialPath).c_str());
	if (error != tinyxml2::XML_SUCCESS) return false;

	tinyxml2::XMLElement* root = document.FirstChildElement("Materials");
	if (root == nullptr) return false;

	UINT32 materialCount = 0;
	for (tinyxml2::XMLElement* node = root->FirstChildElement("Material"); node != nullptr; node = node->NextSiblingElement("Material"))
	{
		materialCount++;
	}

	materials = Array<MaterialInfoVariant>(materialCount);
	materialNames = Array<string>(materialCount);

	// 텍스처 경로는 xml 파일 폴더 기준 상대 경로로 저장되어 있음
	wstring folderPath = std::filesystem::path(materialPath).parent_path().wstring();

	UINT32 index = 0;
	for (tinyxml2::XMLElement* node = root->FirstChildElement("Material"); node != nullptr; node = node->NextSiblingElement("Material"))
	{
		const tinyxml2::XMLElement* nameElement = node->FirstChildElement("Name");
		const char* name = nameElement ? nameElement->GetText() : nullptr;
		materialNames[index] = name ? name : "";

		DefaultMaterialInfo info;
		// 현재 Converter는 BaseColorFile / NormalMapFile, 이전 버전은 DiffuseFile / NormalFile 로 저장
		ReadTextureInfo(node, "BaseColorFile", "DiffuseFile", folderPath, info.textureInfos[(UINT8)eDefaultTextureType::DIFFUSE]);
		ReadTextureInfo(node, "NormalMapFile", "NormalFile", folderPath, info.textureInfos[(UINT8)eDefaultTextureType::NORMAL]);

		info.shaderInfo = shaderInfo;

		materials[index] = info;
		index++;
	}

	return true;
}

bool ModelLoader::LoadMeshes(const wstring& meshPath, const Array<string>& materialNames, OUT ModelData& modelData)
{
	// FileUtils::Open은 실패 시 assert 이므로 먼저 확인
	if (std::filesystem::exists(meshPath) == false) return false;

	// 손상된 파일에서 터무니없는 count로 할당하는 것 방지용
	const UINT64 fileSize = std::filesystem::file_size(meshPath);

	FileUtils file;
	file.Open(meshPath, FileMode::Read);

	// Bone
	// Converter가 루트 노드를 index -1로 저장함 -> 루트(배열 0번)의 자식들도 parent가 -1
	// rootMatrix는 루트 노드의 행렬이 곱해지지 않은 값
	UINT32 boneCount = file.Read<UINT32>();
	if ((UINT64)boneCount * sizeof(Matrix) > fileSize) return false;

	modelData.boneData = Array<BoneData>(boneCount);
	for (UINT32 i = 0; i < boneCount; ++i)
	{
		BoneData& bone = modelData.boneData[i];

		// 파일의 index는 배열 위치와 같아서(루트만 -1) 저장하지 않음
		file.Read<INT32>();
		file.Read(bone.name);
		bone.parentBoneIndex = file.Read<INT32>();
		bone.rootMatrix = file.Read<Matrix>();
	}

	// SubMesh
	UINT32 meshCount = file.Read<UINT32>();
	if (meshCount > fileSize) return false;

	modelData.meshData = Array<SubMeshData>(meshCount);
	for (UINT32 i = 0; i < meshCount; ++i)
	{
		SubMeshData& subMesh = modelData.meshData[i];

		// 노드 이름이라 고유하지 않음 -> 키로 쓰지 않음
		string meshName;
		file.Read(meshName);

		subMesh.boneIndex = file.Read<INT32>();

		string materialName;
		file.Read(materialName);
		subMesh.materialIndex = FindMaterialIndex(materialNames, materialName);

		MeshInfo& meshInfo = subMesh.meshInfo;
		meshInfo.subMeshIndex = (INT32)i;
		meshInfo.filePath = meshPath;

		Geometry& geometry = meshInfo.geometry;
		geometry.vertexSize = (UINT8)sizeof(ModelVertexType);
		geometry.indexSize = eIndexBufferSize::UINT_32;
		geometry.desces = ModelVertexType::GetDesc();

		// Vertex
		UINT32 vertexCount = file.Read<UINT32>();
		UINT64 vertexByteSize = (UINT64)vertexCount * sizeof(ModelVertexType);
		if (vertexByteSize > fileSize) return false;

		geometry.vertices = Array<BYTE>((UINT32)vertexByteSize);
		if (vertexByteSize > 0)
		{
			void* vertexData = geometry.vertices.GetData();
			file.Read(&vertexData, (UINT32)vertexByteSize);
		}

		// Index
		UINT32 indexCount = file.Read<UINT32>();
		UINT64 indexByteSize = (UINT64)indexCount * sizeof(UINT32);
		if (indexByteSize > fileSize) return false;

		geometry.indices = Array<BYTE>((UINT32)indexByteSize);
		if (indexByteSize > 0)
		{
			void* indexData = geometry.indices.GetData();
			file.Read(&indexData, (UINT32)indexByteSize);
		}
	}

	return true;
}

void ModelLoader::ReadTextureInfo(const tinyxml2::XMLElement* materialNode, const char* elementName, const char* legacyElementName, const wstring& folderPath, OUT TextureInfo& textureInfo)
{
	const tinyxml2::XMLElement* element = materialNode->FirstChildElement(elementName);
	if (element == nullptr)
	{
		element = materialNode->FirstChildElement(legacyElementName);
	}
	if (element == nullptr) return;

	const char* fileName = element->GetText();
	// 텍스처가 없으면 비워둠 -> TextureInfo::IsValidFilePath() == false
	if (fileName == nullptr || fileName[0] == '\0') return;

	textureInfo.filePath = (std::filesystem::path(folderPath) / Utils::ToWString(fileName)).wstring();
	textureInfo.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureInfo.mipLevels = 1;
}

INT32 ModelLoader::FindMaterialIndex(const Array<string>& materialNames, const string& materialName)
{
	for (UINT32 i = 0; i < materialNames.GetCount(); ++i)
	{
		if (materialNames[i] == materialName)
		{
			return (INT32)i;
		}
	}

	return INVALID_INDEX;
}
