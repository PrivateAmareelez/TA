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

pair<vector<long long>, vector<int> > dejkstra(int v, vector<list<pair<int, long long> > > &g) {
    int n = g.size();
    vector<long long> d(n, INT64_MAX);
    vector<int> p(n, -1);
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
                p[dest] = curr;
                s.insert(make_pair(d[dest], dest));
            }
        }
    }

    return make_pair(d, p);

}

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

    pair<vector<long long>, vector<int> > path = dejkstra(start, g);

    set<pair<int, int> > edges;
    int temp = finish;
    while (path.second[temp] != -1) {
        edges.insert(make_pair(path.second[temp], temp));
        temp = path.second[temp];
    }

    vector<list<pair<int, long long> > > gExtended(2 * n);
    for (int i = 0; i < n; i++) {
        for (list<pair<int, long long> >::iterator it = g[i].begin(); it != g[i].end(); it++) {
            gExtended[i].push_back(*it);
            gExtended[i + n].push_back(make_pair(it->first + n, it->second));
            if (edges.find(make_pair(i, it->first)) == edges.end())
                gExtended[i].push_back(make_pair(it->first + n, it->second));
        }
    }

    pair<vector<long long>, vector<int> > secondPath = dejkstra(start, gExtended);

    cout << secondPath.first[finish + n] << "\n";

    stack<int> vertexes;
    temp = finish + n;
    vertexes.push(temp >= n ? temp - n : temp);
    while (secondPath.second[temp] != -1) {
        vertexes.push(secondPath.second[temp] >= n ? secondPath.second[temp] - n : secondPath.second[temp]);
        temp = secondPath.second[temp];
    }
    while (vertexes.size() > 1) {
        cout << vertexes.top() + 1 << " ";
        vertexes.pop();
    }
    cout << vertexes.top() + 1 << "\n";

    return 0;
}