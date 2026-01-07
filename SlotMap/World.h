#pragma once
#include "SlotMap.h"
#include "Component.h"
#include "GameObject.h"
#include <memory>
#include <typeinfo>
#include <string>
#include <vector>

using GameObjectHandle = SlotMap<GameObject*>::Handle;
using ComponentHandle = SlotMap<Component*>::Handle;

class World
{
public:

	World();
	~World();

	// GameObject 추가 (World가 소유권 관리)
	GameObjectHandle AddGameObject(const std::string& name);
	bool RemoveGameObject(GameObjectHandle handle);
	GameObject* GetGameObject(GameObjectHandle handle);

	// Component 관리 (World를 통해 생성/제거)
	template<typename T, typename... Args>
	ComponentHandle AddComponentToGameObject(GameObjectHandle objHandle, Args&&... args);

	Component* GetComponent(ComponentHandle handle);
	const Component* GetComponent(ComponentHandle handle) const;
	bool RemoveComponent(ComponentHandle handle);

	// 특정 타입의 모든 Component 가져오기
	template<typename T>
	std::vector<T*> GetComponentsOfType();

	// 모든 GameObject 업데이트
	void UpdateAll();

	// 현재 GameObject/Component 개수
	size_t GetObjectCount() const { return objectMap.size(); }
	size_t GetComponentCount() const { return componentMap.size(); }

private:
	SlotMap<GameObject*> objectMap;
	SlotMap<Component*> componentMap;
};

// 템플릿 함수 구현
template<typename T, typename... Args>
ComponentHandle World::AddComponentToGameObject(GameObjectHandle objHandle, Args&&... args)
{
	static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

	GameObject* obj = GetGameObject(objHandle);
	if (!obj)
		return ComponentHandle{ 0, 0 }; // Invalid handle

	T* comp = new T(std::forward<Args>(args)...);
	comp->owner = obj;

	ComponentHandle compHandle = componentMap.insert(comp);
	obj->RegisterComponent(compHandle);
	comp->OnEnable();

	return compHandle;
}

template<typename T>
std::vector<T*> World::GetComponentsOfType()
{
	std::vector<T*> result;
	for (Component* comp : componentMap.data())
	{
		if (T* casted = dynamic_cast<T*>(comp))
		{
			result.push_back(casted);
		}
	}
	return result;
}
