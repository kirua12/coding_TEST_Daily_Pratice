#include<algorithm>
#include<iostream>
#include<vector>
#include<string>

using namespace std;

vector<int> dwarfs = {};
int sumfix[10] = {0,};

int flag = 0;

void printV(const vector<int>& v) {

	for (auto i : v)
		cout << i << "\n";
}

void combi(int n, int depth, vector<int> ans) {

	if (depth == 7) { // 종료 조건 7명 다 뽑았을 경우
		if (sumfix[depth] == 100) { // sum이 100이면
			printV(ans); //출력
			flag = 1;
		}
		return;
	}

	if (depth > 7)
		return;

	if (flag == 1) // 누적합 100을 찾았을 경우도 return
		return;

	for (int i = n+1; i < 9; i++) {
		ans.push_back(dwarfs[i]);
		sumfix[depth + 1] = dwarfs[i] + sumfix[depth]; // 누적합 sumfix 0번째 인덱스는 0으로
		
		combi(i, depth+1,ans);
		
		sumfix[depth + 1] = 0;
		ans.pop_back();
	}

	return;		
}

int main() {
	
	fill(sumfix, sumfix + 10, 0);

	for (int i = 0; i < 9; i++) {
		int h = 0;
		cin >> h;

		dwarfs.push_back(h);
	}

	sort(dwarfs.begin(), dwarfs.end()); // 오름차수 정렬
	vector<int> ans = {};
	combi(-1, 0, ans);

	return 0;
}