#include <vector>
#include <queue>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("input.txt");
ofstream cout("output.txt");
#else
#include <iostream>
#endif

int cnt = 1;

void bfs(int s, const vector<vector<char> > &g, vector<int> &mark) {
    queue<int> q;
    q.push(s);

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int i = 0; i < g[v].size(); i++) {
            if (g[v][i] == '1' && mark[i] == -1) {
                mark[i] = cnt++;
                q.push(i);
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;

    vector<vector<char> > g(n, vector<char>(n, 0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            cin >> g[i][j];
        }

    vector<int> mark(n, -1);

    for (int i = 0; i < n; i++) {
        if (mark[i] == -1) {
            mark[i] = cnt++;
            bfs(i, g, mark);
        }
    }

    for (int i = 0; i < n; i++) {
        cout << mark[i] << " ";
    }
    cout << "\n";
    return 0;
}