// SlotMap.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "SlotMap.h"
#include "Component.h"
#include "GameObject.h"
#include "World.h"


void TestSlotMapBasic()
{
	using HandleInt = SlotMap<int>::Handle;
	SlotMap<int> sm;

	// 값을 전달해서 내부에서 생성하며 포인터 대신 쓸수 있는 핸들을 리턴
	HandleInt h1 = sm.insert(10);
	HandleInt h2 = sm.insert(20);
	HandleInt h3 = sm.insert(30);
	
	// 핸들로 실제 메모리위치 얻기
	int *ptr1 =  sm.get(h1);
	*ptr1 = 40;
	
	int* ptr2 = sm.get(h2);
	*ptr2 = 50;

	// 핸들로 메모리 해제
	sm.erase(h1);

	// 핸들로 해제된 메모리인지 확인
	bool result = sm.isValid(h1);


	// 빠른 iteration
	int total=0;
	for (int& v : sm.data()) {
		total += v;
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
	}
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

	// Component 정의
	struct HealthComponent : Component
	{
		explicit HealthComponent(int hp) : hp(hp) {}
		int hp;
		void OnEnable() override { std::cout << "HealthComponent OnEnable (hp=" << hp << ")" << std::endl; }
		void OnDestroy() override { std::cout << "HealthComponent OnDestroy" << std::endl; }
	};

	struct TransformComponent : Component
	{
		explicit TransformComponent(float x, float y) : x(x), y(y) {}
		float x, y;
		void OnEnable() override { std::cout << "TransformComponent OnEnable (x=" << x << ", y=" << y << ")" << std::endl; }
		void OnDestroy() override { std::cout << "TransformComponent OnDestroy" << std::endl; }
	};

	// GameObject에 Component 추가 (World를 통해 관리됨)
	std::cout << "\nAdding components to Player..." << std::endl;
	auto healthHandle = world.AddComponentToGameObject<HealthComponent>(player, 100);
	auto transformHandle = world.AddComponentToGameObject<TransformComponent>(player, 10.0f, 20.0f);

	std::cout << "\nComponent count: " << world.GetComponentCount() << std::endl;

	// Component 접근
	if (Component* comp = world.GetComponent(healthHandle))
	{
		HealthComponent* health = dynamic_cast<HealthComponent*>(comp);
		if (health)
		{
			std::cout << "Player health: " << health->hp << std::endl;
			health->hp -= 10;
			std::cout << "Player health after damage: " << health->hp << std::endl;
		}
	}

	// 특정 타입의 모든 Component 가져오기
	std::cout << "\nAll HealthComponents in the world:" << std::endl;
	auto allHealth = world.GetComponentsOfType<HealthComponent>();
	for (HealthComponent* h : allHealth)
	{
		std::cout << "  HP: " << h->hp << " (Owner: " << (h->owner ? h->owner->GetName() : "None") << ")" << std::endl;
	}

	// Enemy1에도 Component 추가
	std::cout << "\nAdding components to Enemy1..." << std::endl;
	world.AddComponentToGameObject<HealthComponent>(enemy1, 50);
	world.AddComponentToGameObject<TransformComponent>(enemy1, 5.0f, 5.0f);

	std::cout << "Component count: " << world.GetComponentCount() << std::endl;

	// Component 제거 테스트
	std::cout << "\nRemoving Player's health component..." << std::endl;
	world.RemoveComponent(healthHandle);
	std::cout << "Component count after removal: " << world.GetComponentCount() << std::endl;

	// GameObject 제거
	std::cout << "\nRemoving Enemy1..." << std::endl;
	world.RemoveGameObject(enemy1);

	std::cout << "Object count after removal: " << world.GetObjectCount() << std::endl;
	std::cout << "Component count after GameObject removal: " << world.GetComponentCount() << std::endl;

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
	TestSlotMapBasic();
	TestSlotMapIteration();
	TestWorld();
}