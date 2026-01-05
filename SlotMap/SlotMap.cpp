// SlotMap.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "SlotMap.h"
#include "World.h"
#include "GameObject.h"


void Test()
{
	SlotMap<int> sm;

	auto h1 = sm.insert(10);
	auto h2 = sm.insert(20);

	*sm.get(h1) = 42;

	sm.erase(h1);

	assert(sm.get(h1) == nullptr); // ❗ 죽은 핸들
	assert(sm.get(h2) != nullptr);

	// 빠른 iteration
	for (int& v : sm.data()) {
		v += 1;
	}
}

void TestSlotMapIteration()
{
	std::cout << "\n=== SlotMap Iteration Test ===" << std::endl;

	SlotMap<int> sm;
	sm.insert(10);
	sm.insert(20);
	sm.insert(30);
	sm.insert(40);

	// 방법 1: data()를 사용한 순회
	std::cout << "\n[방법 1] data()로 순회:" << std::endl;
	for (int& value : sm.data())
	{
		std::cout << value << " ";
	}
	std::cout << std::endl;

	// 방법 2: iterator를 사용한 range-based for loop
	std::cout << "\n[방법 2] range-based for (iterator):" << std::endl;
	for (int& value : sm)
	{
		std::cout << value << " ";
		value += 5; // 값 수정 가능
	}
	std::cout << std::endl;

	// 방법 3: 전통적인 iterator
	std::cout << "\n[방법 3] 전통적인 iterator:" << std::endl;
	for (auto it = sm.begin(); it != sm.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	// 방법 4: const iterator (읽기 전용)
	std::cout << "\n[방법 4] const iterator:" << std::endl;
	const SlotMap<int>& constSm = sm;
	for (const int& value : constSm)
	{
		std::cout << value << " ";
	}SlotMapIteration();
	Test
	std::cout << std::endl;

	std::cout << "\nSlotMap size: " << sm.size() << std::endl;
}

void TestWorld()
{
	std::cout << "\n=== World with SlotMap Test ===" << std::endl;
	
	World world;

	// GameObject 추가
	auto player = world.AddGameObject("Player");
	auto enemy1 = world.AddGameObject("Enemy1");
	auto enemy2 = world.AddGameObject("Enemy2");

	std::cout << "\nObject count: " << world.GetObjectCount() << std::endl;

	// GameObject 접근
	if (GameObject* obj = world.GetGameObject(player))
	{
		std::cout << "Found: " << obj->GetName() << std::endl;
	}

	// GameObject 제거
	std::cout << "\nRemoving Enemy1..." << std::endl;
	world.RemoveGameObject(enemy1);

	std::cout << "Object count after removal: " << world.GetObjectCount() << std::endl;

	// 제거된 핸들로 접근 시도
	if (world.GetGameObject(enemy1) == nullptr)
	{
		std::cout << "Enemy1 handle is now invalid (as expected)" << std::endl;
	}

	// 모든 GameObject 업데이트
	std::cout << "\nUpdating all GameObjects..." << std::endl;
	world.UpdateAll();

	std::cout << "\n=== Test Complete ===" << std::endl;
}

int main()
{
	Test();
	TestWorld();
}