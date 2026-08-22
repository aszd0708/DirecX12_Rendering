#pragma once
#include "Component.h"

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	GameObject();
	~GameObject();

public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void OnDestroy();

public:
	template<typename C>
	shared_ptr<C> GetComponent()
	{
		eComponentType type = C::GetType();
		if (type != eComponentType::Script)
		{
			return static_pointer_cast<C>(_fixedComponent[(int)type]);
		}

		for (shared_ptr<Component>& component : _components)
		{
			shared_ptr<C> target = dynamic_pointer_cast<C>(component);
			if (target != nullptr)
			{
				return target;
			}
		}
		return nullptr;
	}

	template<typename C>
	shared_ptr<C> AddComponent()
	{
		shared_ptr<C> component = make_shared<C>();
		component->SetGameObject(shared_from_this());
		eComponentType type = C::GetType();
		if (type != eComponentType::Script)
		{
			_fixedComponent[type] = component;
		}
		else
		{
			_components.emplace_back(component);
		}
		return component;
	}

private:
	array<shared_ptr<Component>, (int)eComponentType::END> _fixedComponent;
	vector<shared_ptr<Component>> _components;
};

