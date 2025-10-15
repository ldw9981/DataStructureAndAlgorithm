// Dijkstra.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
// BreadthFirstSearch.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Node;
struct Edge {
	int To;
	int Weight = 1; // 가중치, 기본값 1
};

struct Node {
	char Data;
	vector<Edge> Edges;
};

struct Graph {
	vector<Node> Nodes;

	explicit Graph(int n)
		: Nodes(n)
	{
	}

	void SetNode(int i, char c) { Nodes[i].Data = c; }
	void AddEdge(int from, int to , int weight) {
		Nodes[from].Edges.push_back({ to , weight});
	}

	void PrintGraphNodeAndEdge() const {
		for (int i = 0; i < Nodes.size(); ++i) {
			cout << Nodes[i].Data << ":";
			for (auto& e : Nodes[i].Edges)
				cout << " -" << e.To << "," << e.Weight << " ";
			cout << "\n";
		}
	}

	// Daikstra
	void Daikstra(int start)
	{
		vector<int> Visited(Nodes.size(), false); // 방문 안한 노드는 false
		priority_queue<int> Queue;	// 방문할 노드 index 를 저장하는 큐
		Queue.push(start);		// 시작 노드 index 를 큐에 삽입

		while (!Queue.empty())
		{
			int current = Queue.top(); // 큐에서 방문할 노드 index 추출
			Queue.pop();				// 큐에서 추출한 노드 index 제거

			if (Visited[current] == true) // 방문한 노드면 패스
				continue;

			Visited[current] = true; // 방문 처리
			cout << Nodes[current].Data << " ";

			// 현재 노드의 인접 노드들을 큐에 삽입하여 방문 대기
			// 큐이므로 먼저 삽입된 노드가 먼저 방문됨
			for (auto& edge : Nodes[current].Edges)
			{
				int next = edge.To;
				Queue.push(next);
			}
		}
	}
};







int main()
{
	std::cout << "Hello World!\n";

	Graph graph(6);
	
	graph.SetNode(1, '1');
	graph.SetNode(2, '2');
	graph.SetNode(3, '3');
	graph.SetNode(4, '4');
	graph.SetNode(5, '5');

	graph.AddEdge(1,2,8); 
	graph.AddEdge(1,3,3); 
	graph.AddEdge(2,4,4);
	graph.AddEdge(2,5,15); 
	graph.AddEdge(3,4,13); 
	graph.AddEdge(4,5,2);



	graph.PrintGraphNodeAndEdge();

	graph.Daikstra(1);

	return 0;
}
