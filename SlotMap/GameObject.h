#pragma once
#include <string>
#include <vector>
#include "SlotMap.h"

class Component;
class World;

using ComponentHandle = SlotMap<Component*>::Handle;

class GameObject
{
public:
	GameObject(const std::string& name);
	~GameObject();

	const std::string& GetName() const { return name; }
	void Update();

	// Component 핸들 등록 (World가 호출)
	void RegisterComponent(ComponentHandle handle);

	// Component 핸들 해제
	void UnregisterComponent(ComponentHandle handle);

	// GameObject가 가진 모든 Component 제거 (World를 통해)
	void RemoveAllComponents(World& world);

	// GameObject가 가진 Component 핸들 목록 가져오기
	const std::vector<ComponentHandle>& GetComponentHandles() const { return componentHandles; }

private:
	std::string name;
	std::vector<ComponentHandle> componentHandles; // SlotMap 핸들로 관리
};
