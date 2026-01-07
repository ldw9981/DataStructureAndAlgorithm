#include "World.h"
#include "GameObject.h"
#include "Component.h"
#include <iostream>

World::World()
{
	std::cout << "World created" << std::endl;
}

World::~World()
{
	std::cout << "World destroyed - cleaning up GameObjects and Components" << std::endl;

	// 모든 Component 정리
	for (Component* comp : componentMap.data())
	{
		if (comp)
		{
			comp->OnDestroy();
			delete comp;
		}
	}

	// 모든 GameObject 정리
	for (GameObject* obj : objectMap.data())
	{
		delete obj;
	}
}

GameObjectHandle World::AddGameObject(const std::string& name)
{
	GameObject* newObject = new GameObject(name);
	return objectMap.insert(newObject);
}

bool World::RemoveGameObject(GameObjectHandle handle)
{
	GameObject** obj = objectMap.get(handle);
	if (obj && *obj)
	{
		// GameObject가 가진 모든 Component 제거
		(*obj)->RemoveAllComponents(*this);

		delete *obj;
		return objectMap.erase(handle);
	}
	return false;
}

GameObject* World::GetGameObject(GameObjectHandle handle)
{
	GameObject** obj = objectMap.get(handle);
	return obj ? *obj : nullptr;
}

Component* World::GetComponent(ComponentHandle handle)
{
	Component** comp = componentMap.get(handle);
	return comp ? *comp : nullptr;
}

const Component* World::GetComponent(ComponentHandle handle) const
{
	Component* const* comp = componentMap.get(handle);
	return comp ? *comp : nullptr;
}

bool World::RemoveComponent(ComponentHandle handle)
{
	Component** comp = componentMap.get(handle);
	if (comp && *comp)
	{
		(*comp)->OnDestroy();
		delete *comp;
		return componentMap.erase(handle);
	}
	return false;
}

void World::UpdateAll()
{
	for (GameObject* obj : objectMap.data())
	{
		if (obj)
		{
			obj->Update();
		}
	}
}
