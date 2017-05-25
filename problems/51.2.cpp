#include <vector>
#include <list>
#include <queue>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("input.txt");
ofstream cout("output.txt");
#else
#include <iostream>
#endif

vector<char> used;

void bfs(int i, vector<list<int> > &g) {
    used[i] = true;

    queue<int> q;
    q.push(i);

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        for (list<int>::iterator it = g[curr].begin(); it != g[curr].end(); it++) {
            if (used[*it] == false) {
                used[*it] = true;
                q.push(*it);
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;
    vector<list<int> > g(n);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        g[a].push_back(b);
        g[b].push_back(a);
    }

    used.resize(n, false);
    int ans = -1;
    for (int i = 0; i < n; i++)
        if (used[i] == false) {
            ans++;
            bfs(i, g);
        }

    cout << ans << "\n";
    return 0;
}