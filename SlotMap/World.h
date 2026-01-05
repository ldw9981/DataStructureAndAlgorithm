#pragma once
#include "SlotMap.h"
#include <memory>

class GameObject;

using GameObjectHandle = SlotMap<GameObject*>::Handle;

class World
{
public:	

	World();
	~World();

	// GameObject 추가 (World가 소유권 관리)
	GameObjectHandle AddGameObject(const std::string& name);

	// GameObject 제거
	bool RemoveGameObject(GameObjectHandle handle);

	// GameObject 가져오기
	GameObject* GetGameObject(GameObjectHandle handle);
	const GameObject* GetGameObject(GameObjectHandle handle) const;

	// 모든 GameObject 업데이트
	void UpdateAll();

	// 현재 GameObject 개수
	size_t GetObjectCount() const { return objectMap.size(); }

private:
	SlotMap<GameObject*> objectMap;
};

