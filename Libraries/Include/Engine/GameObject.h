#pragma once
#include "Component.h"
#include "CpuPoolManager.h"

class GameObject : public IMemoryBlockHanlde
{
public:
	GameObject();
	~GameObject();

public:
	virtual void SetMemoryHandler(const MemoryBlock& handler) override;

public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void OnDestroy();

public:
	template<typename C>
	C* GetComponent()
	{
		eComponentType type = C::GetType();
		MemoryEntry memoryEntry;
		bool isSuccess = false;
		C* component = nullptr;
		if (type != eComponentType::Script)
		{
			isSuccess = _fixedComponentList->GetMemoryBlock((int)type, memoryEntry);
			assert(isSuccess);

			CpuMemoryPool* pool = CpuPoolManager::GetInstance()->GetMemoryPool(memoryEntry.block._poolID);

			isSuccess = pool->GetObjectByMemoryBlock<C>(memoryEntry.block, &component);
			assert(isSuccess);
		}

		else
		{
			int componentCount = _componentList->GetCount();
			type_index typeIndex = typeid(C);
			for (int i = 0; i < componentCount; ++i)
			{
				isSuccess = _componentList->GetMemoryBlock(i, memoryEntry);
				assert(isSuccess);

				CpuMemoryPool* pool = CpuPoolManager::GetInstance()->GetMemoryPool(memoryEntry.block._poolID);
				if (memoryEntry.type == typeIndex)
				{
					isSuccess = pool->GetObjectByMemoryBlock<C>(memoryEntry.block, &component);
					assert(isSuccess);
					break;
				}
			}
		}

		return component;
	}

	template<typename C>
	C* AddComponent()
	{
		eComponentType type = C::GetType();
		UINT8 poolID = 0;

		switch (type)
		{
			case eComponentType::Renderer:
				poolID = (UINT8)CpuPoolManager::ePoolID::RENDERER;
			break;
			default:
				bool bFindPooID = CpuPoolManager::GetInstance()->GetPoolID(sizeof(C), poolID);
				assert(bFindPooID);
			break;
		}

		CpuMemoryPool* pool = CpuPoolManager::GetInstance()->GetMemoryPool(poolID);

		C* component = nullptr;
		bool isSuccess = pool->GetMemory(&component);
		assert(isSuccess);

		component->SetGameObject(_memoryEntry);
		if (type != eComponentType::Script)
		{
			MemoryEntry memoryEntry;
			memoryEntry.block = component->GetMemoryHandler();
			memoryEntry.type = typeid(C);
			_fixedComponentList->AddAt(memoryEntry, (int)type);
		}
		else
		{
			MemoryEntry memoryEntry;
			memoryEntry.block = component->GetMemoryHandler();
			memoryEntry.type = typeid(C);
			_componentList->Add(memoryEntry);
		}
		return component;
	}

public:
	MemoryEntry& GetMemoryEntry() { return _memoryEntry; }

private:
	MemoryEntry _memoryEntry;
	MemoryList* _fixedComponentList;
	MemoryList* _componentList;
};

