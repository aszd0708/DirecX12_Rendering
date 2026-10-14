#include "pch.h"
#include "MaterialInfo.h"

UINT64 MaterialInfo::GetDescSize(eMaterialType type)
{
	switch (type)
	{
	case eMaterialType::DEFAULT:
		return sizeof(DefaultMaterialTextureDesc);
		break;

	default: assert(false);
	}

	return 0;
}
