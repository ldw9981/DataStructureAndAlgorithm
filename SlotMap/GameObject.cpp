#include "GameObject.h"
#include <iostream>

GameObject::GameObject(const std::string& name) : name(name)
{
	std::cout << "GameObject created: " << name << std::endl;
}

GameObject::~GameObject()
{
	std::cout << "GameObject destroyed: " << name << std::endl;
}

void GameObject::Update()
{
	// GameObject 업데이트 로직
}
