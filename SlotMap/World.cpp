#include "World.h"
#include "GameObject.h"
#include <iostream>

World::World()
{
	std::cout << "World created" << std::endl;
}

World::~World()
{
	std::cout << "World destroyed - cleaning up GameObjects" << std::endl;
	
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

const GameObject* World::GetGameObject(GameObjectHandle handle) const
{
	GameObject* const* obj = objectMap.get(handle);
	return obj ? *obj : nullptr;
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
