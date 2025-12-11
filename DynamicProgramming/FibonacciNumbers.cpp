#include <iostream>
#include <stack>

/*
    https://ko.wikipedia.org/wiki/%ED%94%BC%EB%B3%B4%EB%82%98%EC%B9%98_%EC%88%98
    수학에서 피보나치 수(영어: Fibonacci numbers)는 첫째 및 둘째 항이 1이며
    그 뒤의 모든 항은 바로 앞 두 항의 합인 수열이다.
    처음 여섯 항은 각각 1, 1, 2, 3, 5, 8이다. 편의상 0번째 항을 0으로 두기도 한다.
*/

int fib(int n)
{
	if (n <= 1)
		return n;

	return fib(n - 1) + fib(n - 2);
}

// Memoization 기법을 사용한 피보나치 수열
// 재귀 호출 시 이미 계산된 값은 배열에 저장해두고 다시 계산하지 않음
int fib_memo(int n, int* arr)
{
    if (n <= 1)
        return n;
    if (arr[n] != -1)
        return arr[n];
    
    arr[n] = fib_memo(n - 1, arr) + fib_memo(n - 2, arr);    
    return arr[n];
}

// Tabulation 기법을 사용한 피보나치 수열
// 작은 문제부터 차례대로 해결해 나가면서 배열에 값을 저장
int fib_tab(int n, int* arr)
{
    arr[0] = 0;
    arr[1] = 1;
    for (int i = 2; i <= n; i++)
    {
        arr[i] = arr[i - 1] + arr[i - 2];
    }
    int result = arr[n];
    return result;
}

int fib_memo_stack(int n, int* arr)
{	
	std::stack<int> st;
	st.push(n);

	while (!st.empty()) {
		int x = st.top();

		// 이미 값이 계산되어 있으면 그냥 pop
		if (arr[x] != -1) {
			st.pop();
			continue;
		}

		// 기본 값 (재귀의 if (n <= 1) return n; 부분)
		if (x <= 1) {
			arr[x] = x;
			st.pop();
			continue;
		}

		// 아직 arr[x]를 계산하려면 arr[x-1], arr[x-2]가 필요함
		// 두 자식이 모두 계산된 상태라면 이제 x를 계산할 수 있음
		if (arr[x - 1] != -1 && arr[x - 2] != -1) {
			arr[x] = arr[x - 1] + arr[x - 2];
			st.pop();
		}
		else {
			// 아직 계산 안 된 자식들을 스택에 먼저 넣어서
			// 나중에 x로 다시 돌아왔을 때 이미 값이 채워지도록 함

			if (arr[x - 1] == -1)
				st.push(x - 1);
			if (arr[x - 2] == -1)
				st.push(x - 2);
		}
	}

	return arr[n];
}



#define SIZE 40

int main()
{
    int Array[SIZE] = { 0 };
    memset(Array, -1, sizeof(Array));

    for (int i = 0; i < SIZE; i++)
    {
		std::cout << "fib(" << i << ") = " << fib(i) << std::endl;
        //std::cout << "fib(" << i << ") = " << fib_tab(i,Array) << std::endl;
        //std::cout << "fib(" << i << ") = " << fib_memo(i, Array) << std::endl;
		//std::cout << "fib(" << i << ") = " << fib_memo_stack(i, Array) << std::endl;
		
    }

}
