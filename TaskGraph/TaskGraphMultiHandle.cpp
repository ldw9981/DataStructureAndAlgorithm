#include <functional>
#include <vector>
#include <queue>
#include <iostream>
#include <windows.h> // Sleep �Լ���
#include <thread>

// �̱����� Task�� Thread 1:1 �����̵Ǿ� �۵��Ѵ�.
// �� Handle��ü�� ����� ����� �����Ͽ� �ٸ� Task�� �� �ڵ��� �����ϵ����Ѵ�.
// ������ ���� Task�� 64�� �ۿ� �������� �ʴ´�. WaitForMultipleObjects�� �Ѱ�
// ���� �� ������ �������� 64�� ������ TaskGraph���� ���� �����ϴ�.
// ���� �ڵ�,Thread ����/�ı� ������尡 ũ�Ƿ� ���� ����ϱ� ������ ������ �ƴϴ�. 
// ThreadPool�� �����ϱ� ���� ���� ����� ���÷� ��������.

using namespace std;

class TaskGraph;
TaskGraph* g_graph;

struct TaskNode {
	function<void()> run;    // ������ �Լ� ��ü
	vector<int> next;        // �ļ� ���� (���� u->v)
	int indeg = 0;           // �������� (Kahn �˰��� �뵵, ���⼭�� �����)

	HANDLE completionEvent;  // �� Task�� �Ϸ�Ǿ����� �˸��� WinAPI Event ��ü
	vector<HANDLE> dependencies; // ���� Task���� Event �ڵ� ���
	int id;                  // �ڽ��� ID
};

void RunTaskThread(int task_id);

class TaskGraph {
public:
	vector<TaskNode> nodes;

	// �� ��� �߰�: �Լ� ��ü�� ����ϰ� id ��ȯ
	int AddTask(function<void()> fn) {
		// TaskNode ���� �� Event ��ü�� �Բ� �����մϴ�.
		HANDLE eventHandle = CreateEvent(
			NULL,    // ���� �Ӽ� (�⺻��)
			TRUE,    // Manual-reset Event (SetEvent �� ���� ResetEvent �ʿ�, �ƴϸ� Auto-reset)
			FALSE,   // �ʱ� ����: Non-signaled (�Ϸ���� ���� ����)
			NULL     // �̺�Ʈ �̸� (NULL: �̸� ����)
		);

		int id = (int)nodes.size();
		nodes.push_back(TaskNode{ std::move(fn), {}, 0, eventHandle, {}, id });
		return id;
	}

	// ������ �߰�: u -> v  (u ���� v����)
	void AddDependency(int u, int v) {
		nodes[u].next.push_back(v);
		nodes[v].indeg += 1;
		// u�� v�� ���� Task�̹Ƿ�, v�� u�� Event�� ��ٷ��� �մϴ�.
		nodes[v].dependencies.push_back(nodes[u].completionEvent);
	}

	// Kahn ��� ���� ���� ���, ��Ƽ������ ������ ����
	bool Run() {
		g_graph = this; // ���� ������ ����
		vector<thread> threads;

		// 1. ��� Task�� ���� �����带 �����ϰ� ������ �����մϴ�.
		for (int i = 0; i < (int)nodes.size(); ++i) {
			threads.emplace_back(RunTaskThread, i);
		}

		// 2. ��� Task �����尡 �Ϸ�� ������ ��ٸ��ϴ�.
		for (auto& t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}

		// 3. ����� Event �ڵ��� �ݽ��ϴ�.
		for (auto& node : nodes) {
			CloseHandle(node.completionEvent);
		}

		// WinAPI + std::thread ��Ŀ����� ����Ŭ ������ ���� ���� �ʽ��ϴ�.
		// ����Ŭ�� ������ �����(Deadlock)�� �߻��Ͽ� ������ ����ϰ� �˴ϴ�.
		// (�� �Լ��� ������ ��ȯ���� ���� �� �ֽ��ϴ�)
		return true;
	}

	// �Ҹ��ڿ��� Ȥ�� �� Event �ڵ� ���� (���� ����, Run���� �̹� CloseHandle ��)
	~TaskGraph() {
		if (g_graph == this) g_graph = nullptr;
	}
};

// Task�� �����ϰ� �ϷḦ �˸��� ������ ���� �Լ�
void RunTaskThread(int task_id)
{
	TaskNode& u = g_graph->nodes[task_id];

	// 1. ���� Task���� �Ϸ�Ǳ⸦ ��ٸ��ϴ�.
	// u.dependencies�� Event���� ��� Signal ���°� �� ������ ��ٸ��ϴ�.
	if (!u.dependencies.empty()) {
		DWORD wait_result = WaitForMultipleObjects(
			(DWORD)u.dependencies.size(), // ����� �ڵ��� ����
			u.dependencies.data(),        // �ڵ� �迭
			TRUE,                         // TRUE: ��� �ڵ��� Signal�� �� ������ ��� (AND ����)
			INFINITE                      // ������ ���
		);
		// Error Check (������)
		if (wait_result == WAIT_FAILED) {
			cerr << "Task " << task_id << " WaitForMultipleObjects failed.\n";
			return;
		}
	}

	// 2. Task ����
	cout << "Executing Task " << task_id << "...\n";
	if (u.run) u.run();
	cout << "Task " << task_id << " finished.\n";

	// 3. Task �ϷḦ �˸��ϴ�.
	// �ļ� Task���� ��ٸ��� �ִ� completionEvent�� Signal ���·� ����ϴ�.
	if (!SetEvent(u.completionEvent)) {
		cerr << "Task " << task_id << " SetEvent failed.\n";
	}
}


// ���� main �Լ��� �����ϰ� ��� �����մϴ�.
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

	// g_graph ���� ���� ���� �� ����
	bool ok = g.Run();
	cout << "All tasks completed.\n";

	// ����Ŭ�� ���� ��� g.Run()���� ������ ������ ���� �� �ֽ��ϴ�.
	if (!ok) cerr << "Cycle detected! (������ �׷����� ����Ŭ�� �ֽ��ϴ�)\n";

	return 0;
}