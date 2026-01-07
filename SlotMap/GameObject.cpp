#include "GameObject.h"
#include "World.h"
#include <iostream>
#include <algorithm>

GameObject::GameObject(const std::string& name) : name(name)
{
	std::cout << "GameObject created: " << name << std::endl;
}

GameObject::~GameObject()
{
	// Component는 World의 ComponentPool에서 관리되므로 여기서는 핸들만 정리
	componentHandles.clear();

	std::cout << "GameObject destroyed: " << name << std::endl;
}

void GameObject::RegisterComponent(ComponentHandle handle)
{
	componentHandles.push_back(handle);
}

void GameObject::UnregisterComponent(ComponentHandle handle)
{
	auto it = std::find_if(componentHandles.begin(), componentHandles.end(),
		[&handle](const ComponentHandle& h) {
			return h.slotIndex == handle.slotIndex && h.generation == handle.generation;
		});

	if (it != componentHandles.end())
	{
		componentHandles.erase(it);
	}
}

void GameObject::RemoveAllComponents(World& world)
{
	// 모든 Component를 World를 통해 제거
	for (const ComponentHandle& handle : componentHandles)
	{
		world.RemoveComponent(handle);
	}
	componentHandles.clear();
}

void GameObject::Update()
{
	// GameObject 업데이트 로직
}
