#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <string>
#include <list>
using namespace std;

vector<list<int>> adjC, adjT; // (adjC)정방향과 (adjT)역방향 그래프의 인접 리스트
vector<string> webPages;// 페이지의 주소를 저장하는 변수
vector<bool> visited; // 노드의 방문 여부를 나타내는 변수
stack<int> Stack; // SCC 찾는 스택
vector<int> sccH; // 노드가 속한 SCC의 대표 노드를 저장하는 변수
vector<vector<int>> sccGroups; // SCC 그룹을 저장하는 변수
list<int> sccOrder; // SCC 그룹의 발견 순서를 저장하는 변수

//DFS로 SCC 찾기
void dfs1(int v) {
    visited[v] = true;
    list<int>& adjList = adjC[v];
    adjList.sort(); // 알파벳 순으로 방문하기 위해 정렬

    for (int u : adjList) {
        if (!visited[u]) {
            dfs1(u);
        }
    }
    Stack.push(v); // DFS 종료 후 스택에 정점 추가
}
//역방향 그래프로 SCC찾는 함수
void dfs2(int v, int leader) {
    visited[v] = true;
    sccH[v] = leader; // 정점이 속한 SCC 대표 노드 설정
    sccGroups[leader].push_back(v); // 해당 SCC에 정점 추가

    for (int u : adjT[v]) {
        if (!visited[u]) {
            dfs2(u, leader);
        }
    }
}

int main() {
    int N;
    cin >> N;

    adjC.resize(N);
    adjT.resize(N);
    webPages.resize(N);
    visited.resize(N, false);
    sccH.resize(N);
    sccGroups.resize(N);

    for (int i = 0; i < N; ++i) { //입력받기
        cin >> webPages[i];
    }
    sort(webPages.begin(), webPages.end()); // 페이지를 알파벳 순으로 정렬

    for (int i = 0; i < N; i++) {
        string page;
        int links;
        cin >> page >> links;
        int pageIndex = find(webPages.begin(), webPages.end(), page) - webPages.begin();

        for (int j = 0; j < links; ++j) {
            string link;
            cin >> link;
            int linkIndex = find(webPages.begin(), webPages.end(), link) - webPages.begin();
            adjC[pageIndex].push_back(linkIndex); // 정방향 그래프에 연결 추가
            adjT[linkIndex].push_back(pageIndex); // 역방향 그래프에 연결 추가
        }
    }
    // dfs1으로 스택에 SCC리더를 역순으로 추가함.
    for (int i = 0; i < N; ++i) {
        if (!visited[i]) {
            dfs1(i);
        }
    }

    //스택 내용 출력
    vector<string> temp_vec;
    stack<int> temp_stack = Stack; // 스택을 복사함.
    while (!temp_stack.empty()) {
        temp_vec.push_back(webPages[temp_stack.top()]);
        temp_stack.pop();
    }
    for (auto it = temp_vec.rbegin(); it != temp_vec.rend(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
    //방문 여부 초기화


    fill(visited.begin(), visited.end(), false);
    //스택에서 정점 꺼내면서 dfs2 사용해 SCC 찾음.
    while (!Stack.empty()) {
        int v = Stack.top();
        Stack.pop();
        if (!visited[v]) {
            dfs2(v, v);
            sccOrder.push_back(v); // SCC의 리더 저장
        }
    }
    // 페이지가 포함된 SCC의 리더 출력
    for (string page : webPages) {
        int pageIndex = find(webPages.begin(), webPages.end(), page) - webPages.begin();
        cout << webPages[sccH[pageIndex]] << " ";
    }
    cout << endl;

    return 0;
}
