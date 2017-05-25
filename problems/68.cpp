#include <vector>
#include <list>
#include <queue>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("in.txt");
ofstream cout("out.txt");
#else
#include <iostream>
#endif

pair<int, vector<char> > bfs(int i, vector<list<int> > &g) {
    vector<char> used(g.size(), false);
    vector<int> d(g.size(), INT32_MAX);
    int ans = 0;
    used[i] = true;
    d[i] = 0;

    queue<int> q;
    q.push(i);

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        for (list<int>::iterator it = g[curr].begin(); it != g[curr].end(); it++) {
            if (!used[*it]) {
                used[*it] = true;
                d[*it] = d[i] + 1;
                ans = max(ans, d[*it]);
                q.push(*it);
            }
        }
    }

    return make_pair(ans, used);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;

    vector<list<int> > g(n);
    for (int i = 0; i < n; i++) {
        int m;
        cin >> m;
        for (int j = 0; j < m; j++) {
            int x;
            cin >> x;
            x--;
            g[i].push_back(x);
        }
    }

    pair<int, int> ans = make_pair(-1, -1);
    for (int i = 0; i < n; i++) {
        pair<int, vector<char> > d = bfs(i, g);

        bool fallen = true;
        for (int i = 0; i < d.second.size(); i++)
            if (!d.second[i]) {
                fallen = false;
                break;
            }

        if (fallen && ans.second <= d.first)
            ans = make_pair(i, d.first);
    }

    if (ans.second == -1)
        cout << "impossible\n";
    else {
        cout << ans.second << "\n";
        cout << ans.second << "\n";
    }

    return 0;
}