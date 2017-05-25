#include <vector>
#include <list>
#include <set>
#include <stack>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("input.in");
ofstream cout("output.out");
#else
#include <iostream>
#endif

pair<vector<long long>, vector<pair<int, int> > > dejkstra(int v, vector<list<pair<int, long long> > > &g) {
    int n = g.size();
    vector<long long> d(n, INT64_MAX);
    vector<pair<int, int> > p(n, make_pair(-1, -1));
    set<pair<long long, int> > s;

    d[v] = 0;
    s.insert(make_pair(0, v));

    while (!s.empty()) {
        int curr = s.begin()->second;

        s.erase(s.begin());

        for (list<pair<int, long long> >::iterator it = g[curr].begin(); it != g[curr].end(); it++) {
            int dest = it->first;
            long long cost = it->second;

            if (d[curr] + cost < d[dest]) {
                s.erase(make_pair(d[dest], dest));
                d[dest] = d[curr] + cost;
                p[dest] = make_pair(curr, distance(g[curr].begin(), it));
                s.insert(make_pair(d[dest], dest));
            }
        }
    }

    return make_pair(d, p);

}

vector<list<pair<int, long long> > > reverseGraph(vector<list<pair<int, long long> > > &g) {
    vector<list<pair<int, long long> > > res(g.size());
    for (int i = 0; i < g.size(); i++) {
        for (list<pair<int, long long> >::iterator it = g[i].begin(); it != g[i].end(); it++) {
            res[it->first].push_back(make_pair(i, it->second));
        }
    }
    return res;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    vector<list<pair<int, long long> > > g(n);
    for (int i = 0; i < m; i++) {
        int a, b;
        long long c;
        cin >> a >> b >> c;
        a--;
        b--;
        g[a].push_back(make_pair(b, c));
    }

    int start, finish;
    cin >> start >> finish;
    start--;
    finish--;

    pair<vector<long long>, vector<pair<int, int> > > dFromStart = dejkstra(start, g);
    vector<list<pair<int, long long> > > reversed_g = reverseGraph(g);
    pair<vector<long long>, vector<pair<int, int> > > dFromFinish = dejkstra(finish, reversed_g);

    long long pathLen = dFromStart.first[finish];
    pair<long long, pair<int, int> > ans = make_pair(INT64_MAX, make_pair(-1, -1));
    for (int i = 0; i < n; i++) {
        for (list<pair<int, long long> >::iterator it = g[i].begin(); it != g[i].end(); it++) {
            if (dFromStart.second[it->first].first == i &&
                dFromStart.second[it->first].second == distance(g[i].begin(), it))
                continue;
            if (dFromFinish.first[it->first] == INT64_MAX || dFromStart.first[i] == INT64_MAX)
                continue;
            int temp = it->first;
            long long currLen = dFromStart.first[i] + it->second + dFromFinish.first[it->first];
            if (currLen < ans.first)
                ans = make_pair(currLen, make_pair(i, it->first));
        }
    }

    cout << ans.first << "\n";
    int temp = ans.second.first;
    stack<int> s;
    while (dFromStart.second[temp].first != -1) {
        s.push(dFromStart.second[temp].first + 1);
        temp = dFromStart.second[temp].first;
    }
    while (!s.empty()) {
        int v = s.top();
        s.pop();
        cout << v << " ";
    }
    cout << ans.second.first + 1 << " " << ans.second.second + 1;
    temp = ans.second.second;
    while (dFromFinish.second[temp].first != -1) {
        cout << " " << dFromFinish.second[temp].first + 1;
        temp = dFromFinish.second[temp].first;
    }
    cout << "\n";
    return 0;
}