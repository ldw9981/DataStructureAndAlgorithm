// BreadthFirstSearch.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Node;
struct Edge {
	int To; 
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
	void AddDirectedEdge(int from, int to) 
	{
		Nodes[from].Edges.push_back({ to });
	}

	void AddUndirectedEdge(int a, int b) 
	{
		Nodes[a].Edges.push_back({ b });
		Nodes[b].Edges.push_back({ a });
	}


	void PrintGraphNodeAndEdge() 
	{	
		cout << "PrintGraphNodeAndEdge\n";
		for (int i = 0; i < Nodes.size(); ++i) {
			cout << Nodes[i].Data << ":";
			for (auto& e : Nodes[i].Edges)
				cout << " -" << Nodes[e.To].Data;
			cout << "\n";
		}
	}

	// BFS 함수
	void PrintBFS(int start)
	{
		cout << "PrintBFS\n";
		if (start < 0 || start >= (int)Nodes.size()) 
			return;

		vector<bool> Visit(Nodes.size(), false); // 방문 안한 노드는 false
		queue<int> Queue;	// 방문할 노드 index 를 저장하는 큐
		Visit[start] = true;
		Queue.push(start);		// 시작 노드 index 를 큐에 삽입

		while (!Queue.empty())
		{
			int current = Queue.front(); // 큐에서 방문할 노드 index 추출
			Queue.pop();				// 큐에서 추출한 노드 index 제거						
					
			cout << Nodes[current].Data << " ";

			// 현재 노드의 인접 노드들을 큐에 삽입하여 방문 대기
			// 큐이므로 먼저 삽입된 노드가 먼저 방문됨
			// 삽입할때 방문처리해야 중복삽입 안됨.
			for (auto& edge : Nodes[current].Edges)
			{
				int next = edge.To;
				if (Visit[next] == true)
					continue;

				Visit[next] = true;	// 방문 처리			
				Queue.push(next);					
			}
		}
	}
};







int main()
{
    std::cout << "Hello World!\n";

	Graph graph(8);
	graph.SetNode(0, 'S');
	graph.SetNode(1, 'A');
	graph.SetNode(2, 'B');
	graph.SetNode(3, 'C');
	graph.SetNode(4, 'D');
	graph.SetNode(5, 'E');
	graph.SetNode(6, 'F');
	graph.SetNode(7, 'G');

	// 무방향이면 양쪽으로 연결한다.
	graph.AddUndirectedEdge(0, 1); // S-A
	graph.AddUndirectedEdge(0, 2); // S-B
	graph.AddUndirectedEdge(0, 3); // S-C

	graph.AddUndirectedEdge(1, 4); // A-D
	graph.AddUndirectedEdge(2, 5); // B-E
	graph.AddUndirectedEdge(3, 6); // C-F
	graph.AddUndirectedEdge(4, 7); // D-G
	graph.AddUndirectedEdge(5, 7); // E-G
	graph.AddUndirectedEdge(6, 7); // F-G


	graph.PrintGraphNodeAndEdge();

	graph.PrintBFS(0);

	return 0;
}
