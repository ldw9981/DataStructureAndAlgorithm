#include <functional>
#include <vector>
#include <queue>
#include <iostream>
#include <windows.h> // Sleep 함수용
#include <thread>

// 이구조는 Task와 Thread 1:1 연결이되어 작동한다.
// 즉 Handle객체를 만들어 종료시 설정하여 다른 Task가 그 핸들을 참조하도록한다.
// 문제는 의존 Task는 64개 밖에 지원하지 않는다. WaitForMultipleObjects의 한계
// 따라서 이 구현은 의존성이 64개 이하인 TaskGraph에만 적용 가능하다.
// 또한 핸들,Thread 생성/파괴 오버헤드가 크므로 실제 사용하기 적합한 구조는 아니다. 
// ThreadPool을 도입하기 전에 개념 증명용 샘플로 생각하자.

using namespace std;

class TaskGraph;
TaskGraph* g_graph;

struct TaskNode {
	function<void()> run;    // 실행할 함수 객체
	vector<int> next;        // 후속 노드들 (간선 u->v)
	int indeg = 0;           // 진입차수 (Kahn 알고리즘 용도, 여기서는 참고용)

	HANDLE completionEvent;  // 이 Task가 완료되었음을 알리는 WinAPI Event 객체
	vector<HANDLE> dependencies; // 선행 Task들의 Event 핸들 목록
	int id;                  // 자신의 ID
};

void RunTaskThread(int task_id);

class TaskGraph {
public:
	vector<TaskNode> nodes;

	// 새 노드 추가: 함수 객체를 등록하고 id 반환
	int AddTask(function<void()> fn) {
		// TaskNode 생성 시 Event 객체도 함께 생성합니다.
		HANDLE eventHandle = CreateEvent(
			NULL,    // 보안 속성 (기본값)
			TRUE,    // Manual-reset Event (SetEvent 후 직접 ResetEvent 필요, 아니면 Auto-reset)
			FALSE,   // 초기 상태: Non-signaled (완료되지 않은 상태)
			NULL     // 이벤트 이름 (NULL: 이름 없음)
		);

		int id = (int)nodes.size();
		nodes.push_back(TaskNode{ std::move(fn), {}, 0, eventHandle, {}, id });
		return id;
	}

	// 의존성 추가: u -> v  (u 이후 v실행)
	void AddDependency(int u, int v) {
		nodes[u].next.push_back(v);
		nodes[v].indeg += 1;
		// u는 v의 선행 Task이므로, v는 u의 Event를 기다려야 합니다.
		nodes[v].dependencies.push_back(nodes[u].completionEvent);
	}

	// Kahn 방식 위상 실행 대신, 멀티스레드 의존성 실행
	bool Run() {
		g_graph = this; // 전역 포인터 설정
		vector<thread> threads;

		// 1. 모든 Task에 대해 스레드를 생성하고 실행을 시작합니다.
		for (int i = 0; i < (int)nodes.size(); ++i) {
			threads.emplace_back(RunTaskThread, i);
		}

		// 2. 모든 Task 스레드가 완료될 때까지 기다립니다.
		for (auto& t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}

		// 3. 사용한 Event 핸들을 닫습니다.
		for (auto& node : nodes) {
			CloseHandle(node.completionEvent);
		}

		// WinAPI + std::thread 방식에서는 사이클 검출을 따로 하지 않습니다.
		// 사이클이 있으면 데드락(Deadlock)이 발생하여 영원히 대기하게 됩니다.
		// (이 함수가 영원히 반환되지 않을 수 있습니다)
		return true;
	}

	// 소멸자에서 혹시 모를 Event 핸들 정리 (생략 가능, Run에서 이미 CloseHandle 함)
	~TaskGraph() {
		if (g_graph == this) g_graph = nullptr;
	}
};

// Task를 실행하고 완료를 알리는 스레드 진입 함수
void RunTaskThread(int task_id)
{
	TaskNode& u = g_graph->nodes[task_id];

	// 1. 선행 Task들이 완료되기를 기다립니다.
	// u.dependencies의 Event들이 모두 Signal 상태가 될 때까지 기다립니다.
	if (!u.dependencies.empty()) {
		DWORD wait_result = WaitForMultipleObjects(
			(DWORD)u.dependencies.size(), // 대기할 핸들의 개수
			u.dependencies.data(),        // 핸들 배열
			TRUE,                         // TRUE: 모든 핸들이 Signal이 될 때까지 대기 (AND 조건)
			INFINITE                      // 무한정 대기
		);
		// Error Check (선택적)
		if (wait_result == WAIT_FAILED) {
			cerr << "Task " << task_id << " WaitForMultipleObjects failed.\n";
			return;
		}
	}

	// 2. Task 실행
	cout << "Executing Task " << task_id << "...\n";
	if (u.run) u.run();
	cout << "Task " << task_id << " finished.\n";

	// 3. Task 완료를 알립니다.
	// 후속 Task들이 기다리고 있는 completionEvent를 Signal 상태로 만듭니다.
	if (!SetEvent(u.completionEvent)) {
		cerr << "Task " << task_id << " SetEvent failed.\n";
	}
}


// 기존 main 함수는 동일하게 사용 가능합니다.
int main() {
	TaskGraph g;

	int t1 = g.AddTask([] {
		Sleep(10);
		cout << "Task1: Initial task (sleep 10ms)\n";
		});
	int t2 = g.AddTask([] {
		Sleep(1000);
		cout << "Task2: Long task (sleep 1000ms)\n";
		});
	int t3 = g.AddTask([] {
		Sleep(1000);
		cout << "Task3: Long task (sleep 1000ms)\n";
		});
	int t4 = g.AddTask([] {
		cout << "Task4: Dependent task (after 2 & 3)\n";
		});

	// t1 ---> t2 ---> t4
	//    +-> t3 -+
	g.AddDependency(t1, t2);
	g.AddDependency(t1, t3);
	g.AddDependency(t2, t4);
	g.AddDependency(t3, t4);

	// g_graph 전역 변수 설정 및 실행
	bool ok = g.Run();
	cout << "All tasks completed.\n";

	// 사이클이 있을 경우 g.Run()에서 영원히 끝나지 않을 수 있습니다.
	if (!ok) cerr << "Cycle detected! (의존성 그래프에 사이클이 있습니다)\n";

	return 0;
}