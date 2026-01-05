#pragma once
#include <string>

class GameObject
{
public:
	GameObject(const std::string& name);
	~GameObject();

	const std::string& GetName() const { return name; }
	void Update();

private:
	std::string name;
};
