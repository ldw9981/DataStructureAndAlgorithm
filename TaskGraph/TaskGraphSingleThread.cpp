#include <functional>
#include <vector>
#include <queue>
#include <iostream>
#include <windows.h> // Sleep 함수용

using namespace std;

struct TaskNode {
	function<void()> run;     // 실행할 함수 객체
	vector<int> next;         // 후속 노드들 (간선 u->v)
	int indeg = 0;            // 진입차수 (선행 노드 수)
};

struct TaskGraph {
	vector<TaskNode> nodes;

	// 새 노드 추가: 함수 객체를 등록하고 id 반환
	int AddTask(function<void()> fn) {
		nodes.push_back(TaskNode{ std::move(fn), {}, 0 });
		return (int)nodes.size() - 1;
	}

	// 의존성 추가: u -> v  (v가 u 이후 실행)
	void AddDependency(int u, int v) {
		nodes[u].next.push_back(v);
		nodes[v].indeg += 1;
	}

	// Kahn 방식 위상 실행 (단일 스레드 버전)
	bool Run() {
		queue<int> ready;
		for (int i = 0; i < (int)nodes.size(); ++i)
			if (nodes[i].indeg == 0) ready.push(i);

		int visited = 0;
		while (!ready.empty()) {
			int u = ready.front(); ready.pop();
			// 1) 실행
			if (nodes[u].run) nodes[u].run();
			++visited;

			// 2) 후속 indegree 감소 → 0이면 ready
			for (int v : nodes[u].next) {
				if (--nodes[v].indeg == 0) ready.push(v);
			}
		}
		// 모든 노드를 실행했는지(사이클 없는지) 검사
		return visited == (int)nodes.size();
	}
};



int main() 
{
	TaskGraph g;

	int t1 = g.AddTask([] {
		Sleep(10);
		cout << "Task1\n"; 
		});
	int t2 = g.AddTask([] {
		Sleep(1000);
		cout << "Task2\n";
		});
	int t3 = g.AddTask([] {
		Sleep(1000);
		cout << "Task3\n";
		});
	int t4 = g.AddTask([] { cout << "Task4 (after 2 & 3)\n"; });
	/*
		t1 ---> t2 ---> t4
		    +-> t3 -+
	*/
	g.AddDependency(t1, t2); // t2 depends on t1
	g.AddDependency(t1, t3); // t3 depends on t1
	g.AddDependency(t2, t4); // t4 depends on t2
	g.AddDependency(t3, t4); // t4 depends on t3

	bool ok = g.Run();
	if (!ok) cerr << "Cycle detected! (의존성 그래프에 사이클이 있습니다)\n";
}
