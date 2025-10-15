// PriorityQueue.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

#include <queue>


struct Product
{
    std::string name;
    int price;

	bool operator>(const Product& o) const { return price > o.price; } // 작은 price가 top
};

int main()
{
    // 우선 순위큐는 기본적으로 std::less<T> 비교자를 사용하여 operator<를 찾는다.
    //operator< 는 가 true를 반환할 때 그 왼쪽 값이 덜 우선이라 힙 아래쪽(뒤쪽)에 배치된다는 뜻
	// 따라서, 기본적으로 큰 값이 우선순위가 높다. 즉, max-heap 으로 작동

	// min-heap 으로 우선순위 큐 생성 하려면 std::greater 사용한다.  전역함수나, 람다식도 가능
	std::priority_queue<Product,std::vector<Product>,std::greater<Product>> pq;
	pq.push({ "Apple", 5 });
	pq.push({ "Banana", 2 });
	pq.push({ "Orange", 3 });
	pq.push({ "Grapes", 4 });
	pq.push({ "Mango", 1 });

	// 하나씩 꺼내면서 출력하기
	for (; !pq.empty(); pq.pop())
	{
		const Product& p = pq.top();
		std::cout << p.name << ": " << p.price << "\n";
	}
}
