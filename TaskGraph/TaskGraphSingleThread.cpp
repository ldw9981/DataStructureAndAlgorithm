#include <functional>
#include <vector>
#include <queue>
#include <iostream>
#include <windows.h> // Sleep 함수용

using namespace std;

// 단일 스레드용 TaskGraph 구현 (Kahn 위상정렬 방식)
// TaskNode들은 반복 실행이 가능하게 수정하지 않고 처리함.
struct TaskNode {
	function<void()> run;     // 실행할 함수 객체
	vector<int> next;         // 후속 노드들 (간선 u->v)
	int indeg = 0;            // 진입차수 (선행 노드 수)
};

struct TaskGraph {
	vector<TaskNode> nodes;
	queue<int> ready_queue;		// BFS 탐색용 큐-진입차수가 0인 노드들 저장한다.
	vector<int> current_indeg;	// 실행 중 변하는 진입차수 복사본	

	// 새 노드 추가: 함수 객체를 등록하고 id 반환
	int AddTask(function<void()> fn) 
	{	// 함수 객체 이동생성자로 저장
		nodes.push_back(TaskNode{ std::move(fn), {}, 0 });
		return (int)nodes.size() - 1;
	}

	// 의존성 추가: u -> v  (u 이후 v실행)
	void AddDependency(int u, int v) 
	{
		nodes[u].next.push_back(v);	// id 추가
		nodes[v].indeg += 1;		// v의 진입차수 증가
	}

	// Kahn 방식 위상 실행 (단일 스레드 버전)
	void Run() 
	{
		// 현재 진입차수 복사
		current_indeg.resize(nodes.size());		
		for (int i = 0; i < (int)nodes.size(); ++i)
			current_indeg[i] = nodes[i].indeg;
		
		//1) 진입차수 0인 노드들 id 큐에 삽입
		for (int i = 0; i < (int)nodes.size(); ++i)
			if (nodes[i].indeg == 0) ready_queue.push(i);

		while (!ready_queue.empty()) 
		{
			int u = ready_queue.front(); ready_queue.pop();
			// 2) id로 노드 실행
			if (nodes[u].run) 
				nodes[u].run();		

			// 3) 후속노드 indegree 감소 → 0이면 ready
			for (int v : nodes[u].next) {
				if (--current_indeg[v] == 0) ready_queue.push(v);
			}
		}
	}
};



int main() 
{
	TaskGraph g;

	int t1 = g.AddTask([] {
		Sleep(10);
		cout << "Task1 End\n"; 
		});
	int t2 = g.AddTask([] {
		Sleep(1000);
		cout << "Task2 End\n";
		});
	int t3 = g.AddTask([] {
		Sleep(1000);
		cout << "Task3 End\n";
		});
	int t4 = g.AddTask([] {		
		Sleep(1000);
		cout << "Task4 End\n";
		});

	int t5= g.AddTask([] { cout << "Task5 (after 2 & 3 & 4)\n"; });
	/*
		t1 ---> t2 ---> t5
		    +-> t3 -+
			+-> t4 -+
	*/
	g.AddDependency(t1, t2); // t2 depends on t1
	g.AddDependency(t1, t3); // t3 depends on t1
	g.AddDependency(t1, t4); // t3 depends on t1
	g.AddDependency(t2, t5); // t5 depends on t2
	g.AddDependency(t3, t5); // t5 depends on t3
	g.AddDependency(t4, t5); // t5 depends on t4


	// 그래프 자체를 수정하지 않으므로 다시 실행해도 동일하게 동작함
	g.Run();
	g.Run(); 
}
