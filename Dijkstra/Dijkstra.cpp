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
	void AddDirectedEdge(int from, int to,int weight)
	{
		Nodes[from].Edges.push_back({ to,weight });
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
		vector<bool> Visit(Nodes.size(), false); // 방문 안한 노드는 false		
		vector<int> Distance(Nodes.size(),INT_MAX);

		struct q_data
		{
			int NodeIndex;
			int AccumulatedWeight;	//	누적 가중치				
			bool operator>(const q_data& other) const
			{
				return AccumulatedWeight > other.AccumulatedWeight;
			}
		};

		// 방문할 노드 index 를 저장하는 큐
		// greater<T>는 “작은 값이 앞선다”-> min-heap
		priority_queue<q_data, vector<q_data>, std::greater<q_data>> Queue;	
		
		Queue.push({ start,0});		// 시작 노드 index와 가중치	0을 삽입
		Distance[start] = 0;

		while (!Queue.empty())
		{
			auto curr = Queue.top(); // 큐에서 방문할 노드 정보 복사
			Queue.pop();			  // 큐에서 제거
			cout << "top " << curr.NodeIndex << "\n";		

			if( Visit[curr.NodeIndex])
				continue; // 이미 방문한 노드면 무시	

			if( Distance[curr.NodeIndex] < curr.AccumulatedWeight)
				continue; // 이미 더 짧은 경로를 찾있으면 무시

			Visit[curr.NodeIndex] = true; // 방문 처리

			// 현재 노드의 인접 노드들을 순회
			for (auto& edge : Nodes[curr.NodeIndex].Edges)
			{
				cout << "edge " << edge.To << "," << edge.Weight << "\n";
				// 현재 노드를 거쳐서 다음 노드로 가는 비용이 더 적으면 거리값 갱신
				int newCost = Distance[curr.NodeIndex] + edge.Weight;
				if (newCost < Distance[edge.To])
				{
					Distance[edge.To] = newCost;	
					cout << "push " << edge.To << "," << newCost << "\n";
					Queue.push({ edge.To,newCost });	//복사본 보관		
				}	
			}
		}

		// 결과 출력
		for (int i = 0; i < Distance.size(); ++i)
		{
			if (Distance[i] == INT_MAX)
				cout << Nodes[i].Data << ": " << "INF\n";
			else
				cout << Nodes[i].Data << ": " << Distance[i] << "\n";
		}
	}
};







int main()
{
	std::cout << "Hello World!\n";

	Graph graph(6);
	// 번호 맞추기 힘들들어서 0번 노드는 안씀
	graph.SetNode(1, '1');
	graph.SetNode(2, '2');
	graph.SetNode(3, '3');
	graph.SetNode(4, '4');
	graph.SetNode(5, '5');

	graph.AddDirectedEdge(1,2,8);
	graph.AddDirectedEdge(1,3,3);
	graph.AddDirectedEdge(2,4,4);
	graph.AddDirectedEdge(2,5,15);
	graph.AddDirectedEdge(3,4,13);
	graph.AddDirectedEdge(4,5,2);



	graph.PrintGraphNodeAndEdge();

	graph.Daikstra(1);

	return 0;
}
