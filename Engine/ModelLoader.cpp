#include "pch.h"
#include "ModelLoader.h"
#include "FileUtils.h"
#include "Utils.h"
#include "VertexData.h"
#include "tinyxml2.h"
#include <filesystem>

static_assert(sizeof(ModelVertexType) <= UINT8_MAX, "Geometry::vertexSize가 UINT8");
static_assert(sizeof(KeyframeData) == 44, "Converter asKeyframeData와 크기가 같아야 함");

bool ModelLoader::LoadModel(const wstring& meshPath, const wstring& materialPath, const ShaderInfo& shaderInfo, eMaterialType materialType, OUT ModelData& modelData)
{
	// Mesh가 materialName으로 Material을 찾기 때문에 Material 먼저 로드
	Array<string> materialNames;
	bool isSuccess = LoadMaterials(materialPath, shaderInfo, materialType, modelData.materials, materialNames);
	if (isSuccess == false) return false;

	isSuccess = LoadMeshes(meshPath, materialNames, modelData);
	if (isSuccess == false) return false;

	modelData.name = std::filesystem::path(meshPath).stem().string();

	return true;
}

bool ModelLoader::LoadAnimation(const wstring& clipPath, OUT AnimationData& animationData)
{
	// FileUtils::Open은 실패 시 assert 이므로 먼저 확인
	if (std::filesystem::exists(clipPath) == false) return false;

	// 손상된 파일에서 터무니없는 count로 할당하는 것 방지용
	const UINT64 fileSize = std::filesystem::file_size(clipPath);

	FileUtils file;
	file.Open(clipPath, FileMode::Read);

	string clipName;
	file.Read(clipName);

	animationData.name = std::filesystem::path(clipPath).stem().string();
	animationData.duration = file.Read<float>();
	animationData.frameRate = file.Read<float>();
	animationData.frameCount = file.Read<UINT32>();
	if (animationData.frameRate <= 0.f || animationData.frameCount == 0) return false;

	UINT32 trackCount = file.Read<UINT32>();
	if (trackCount > fileSize) return false;

	animationData.tracks = Array<AnimationTrack>(trackCount);
	for (UINT32 i = 0; i < trackCount; ++i)
	{
		AnimationTrack& track = animationData.tracks[i];
		file.Read(track.boneName);

		// 샘플링할 때 모든 트랙을 같은 프레임 인덱스로 읽음
		UINT32 keyCount = file.Read<UINT32>();
		if (keyCount != animationData.frameCount) return false;

		UINT64 keyByteSize = (UINT64)keyCount * sizeof(KeyframeData);
		if (keyByteSize > fileSize) return false;

		track.keyframes = Array<KeyframeData>(keyCount);
		void* keyData = track.keyframes.GetData();
		file.Read(&keyData, (UINT32)keyByteSize);
	}

	return true;
}

bool ModelLoader::LoadMaterials(const wstring& materialPath, const ShaderInfo& shaderInfo, eMaterialType materialType, OUT Array<MaterialInfo>& materials, OUT Array<string>& materialNames)
{
	// 호출하는 쪽이 잘못된 타입을 넘긴 것은 프로그래머 실수
	bool isSupportedType = materialType == eMaterialType::DEFAULT || materialType == eMaterialType::PBR;
	assert(isSupportedType);
	if (isSupportedType == false) return false;

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

	materials = Array<MaterialInfo>(materialCount);
	materialNames = Array<string>(materialCount);

	// 텍스처 경로는 xml 파일 폴더 기준 상대 경로로 저장되어 있음
	wstring folderPath = std::filesystem::path(materialPath).parent_path().wstring();

	// 색 데이터(BaseColor/Diffuse, Emissive)와 수치 데이터(Normal, Metallic, Roughness, AO)를 구분
	// TODO(sRGB 단계) : colorFormat을 DXGI_FORMAT_R8G8B8A8_UNORM_SRGB로 바꾸고 출력 감마 처리
	//                   지금 바꾸면 감마 처리 전이라 기존 모델 색이 어두워짐
	const DXGI_FORMAT colorFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	const DXGI_FORMAT dataFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	UINT32 index = 0;
	for (tinyxml2::XMLElement* node = root->FirstChildElement("Material"); node != nullptr; node = node->NextSiblingElement("Material"))
	{
		const tinyxml2::XMLElement* nameElement = node->FirstChildElement("Name");
		const char* name = nameElement ? nameElement->GetText() : nullptr;
		materialNames[index] = name ? name : "";

		MaterialInfo info = {};
		info.materialType = materialType;
		info.shaderInfo = shaderInfo;

		// 현재 Converter는 BaseColorFile / NormalMapFile, 이전 버전은 DiffuseFile / NormalFile 로 저장
		switch (materialType)
		{
		case eMaterialType::DEFAULT:
		{
			info.textureCount = (UINT8)eDefaultTextureType::MAX;
			ReadTextureInfo(node, "BaseColorFile", "DiffuseFile", folderPath, colorFormat, info.textureInfos[(UINT8)eDefaultTextureType::DIFFUSE]);
			ReadTextureInfo(node, "NormalMapFile", "NormalFile", folderPath, dataFormat, info.textureInfos[(UINT8)eDefaultTextureType::NORMAL]);
			break;
		}
		case eMaterialType::PBR:
		{
			// glTF처럼 Metallic/Roughness가 한 장에 묶인 경우 두 칸에 같은 파일명이 들어옴
			// -> 같은 경로 + 같은 format이라 TextureInfo 해시가 같아서 GPU 리소스는 하나만 생김
			// AO는 Converter가 저장하지 않음 -> 비워둠
			info.textureCount = (UINT8)ePBRTextureType::MAX;
			ReadTextureInfo(node, "BaseColorFile", "DiffuseFile", folderPath, colorFormat, info.textureInfos[(UINT8)ePBRTextureType::BASE_COLOR]);
			ReadTextureInfo(node, "EmissiveMapFile", nullptr, folderPath, colorFormat, info.textureInfos[(UINT8)ePBRTextureType::EMISSIVE]);
			ReadTextureInfo(node, "MetallicMapFile", nullptr, folderPath, dataFormat, info.textureInfos[(UINT8)ePBRTextureType::METALIC]);
			ReadTextureInfo(node, "RoughnessMapFile", nullptr, folderPath, dataFormat, info.textureInfos[(UINT8)ePBRTextureType::ROUGHNESS]);
			ReadTextureInfo(node, "NormalMapFile", "NormalFile", folderPath, dataFormat, info.textureInfos[(UINT8)ePBRTextureType::NORMAL]);
			break;
		}
		default:
			break;
		}

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

			subMesh.isSkinned = HasBlendWeight(vertexData, vertexCount);
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

void ModelLoader::ReadTextureInfo(const tinyxml2::XMLElement* materialNode, const char* elementName, const char* legacyElementName, const wstring& folderPath, DXGI_FORMAT format, OUT TextureInfo& textureInfo)
{
	const tinyxml2::XMLElement* element = materialNode->FirstChildElement(elementName);
	// FirstChildElement(nullptr)은 이름 상관없이 첫 자식을 돌려주므로 nullptr이면 건너뜀
	if (element == nullptr && legacyElementName != nullptr)
	{
		element = materialNode->FirstChildElement(legacyElementName);
	}
	if (element == nullptr) return;

	const char* fileName = element->GetText();
	// 텍스처가 없으면 비워둠 -> TextureInfo::IsValidFilePath() == false
	if (fileName == nullptr || fileName[0] == '\0') return;

	textureInfo.filePath = (std::filesystem::path(folderPath) / Utils::ToWString(fileName)).wstring();
	textureInfo.format = format;
	textureInfo.mipLevels = 1;
}

bool ModelLoader::HasBlendWeight(const void* vertexData, UINT32 vertexCount)
{
	const ModelVertexType* vertices = (const ModelVertexType*)vertexData;

	// Converter는 가중치 없는 정점을 (0, 0, 0, 0)으로 저장함
	for (UINT32 i = 0; i < vertexCount; ++i)
	{
		const Vec4& weights = vertices[i].blendWeights;
		if (weights.x > 0.f || weights.y > 0.f || weights.z > 0.f || weights.w > 0.f)
		{
			return true;
		}
	}

	return false;
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
