#include <vector>
#include <queue>
#include <list>
#include <algorithm>
#include <assert.h>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("input.in");
ofstream cout("output.out");
#else

#include <iostream>

#endif

vector<bool> used;
vector<list<int> > g;
vector<int> matching;
vector<int> layers;
vector<int> u, v, way, mat;
vector<vector<int> > g_matrix;

bool dfs(int v, bool edgeInMatching) {
    used[v] = true;

    if (v < matching.size() / 2 && matching[v] == -1)
        return true;

    for (list<int>::iterator it = g[v].begin(); it != g[v].end(); it++) {
        int temp = *it;

        if (used[*it] || layers[*it] != layers[v] - 1) {
            continue;
        }

        if (edgeInMatching) {
            if (matching[v] != *it && dfs(*it, !edgeInMatching)) {
                matching[v] = *it;
                matching[*it] = v;
                return true;
            }
        } else {
            if (matching[v] == *it && dfs(*it, !edgeInMatching)) {
                return true;
            }
        }
    }

    return false;
}

int curr_checked_vertex = -1;

bool minimizing_dfs(int v, bool edgeInMatching, int next, bool initial) {
    used[v] = true;

    if (v >= matching.size() / 2 && matching[v] == -1)
        return true;

    for (list<int>::iterator it = g[v].begin(); it != g[v].end(); it++) {
        if (initial && next != *it)
            continue;

        int temp = *it;
        if (used[*it] || *it < curr_checked_vertex)
            continue;

        if (edgeInMatching) {
            if (matching[v] != *it && minimizing_dfs(*it, !edgeInMatching, 0, false)) {
                matching[v] = *it;
                matching[*it] = v;
                return true;
            }
        } else {
            if (matching[v] == *it && minimizing_dfs(*it, !edgeInMatching, 0, false)) {
                return true;
            }
        }
    }

    return false;
}

void makeMatching() {
    int n = g.size() / 2;
    matching.assign(2 * n, -1);
    used.assign(2 * n, false);
    layers.assign(2 * n, -1);

    while (true) {
        used.assign(2 * n, false);
        layers.assign(2 * n, -1);
        int k = -1;

        queue<int> q;
        for (int i = 0; i < n; i++) {
            if (matching[i] == -1) {
                q.push(i);
                used[i] = true;
                layers[i] = 0;
            }
        }

        while (!q.empty()) {
            int curr = q.front();
            q.pop();

            if (curr >= n && matching[curr] == -1) {
                k = layers[curr];
                break;
            }

            for (list<int>::iterator it = g[curr].begin(); it != g[curr].end(); it++) {
                if (curr < n) {
                    bool temp = used[*it];
                    if (matching[curr] != *it && !used[*it]) {
                        q.push(*it);
                        used[*it] = true;
                        temp = used[*it];
                        layers[*it] = layers[curr] + 1;
                    }
                } else if (matching[curr] == *it && !used[*it]) {
                    q.push(*it);
                    used[*it] = true;
                    layers[*it] = layers[curr] + 1;
                }
            }

        }

        if (k == -1)
            return;

        used.assign(2 * n, false);

        for (int i = 0; i < 2 * n; i++)
            if (layers[i] == k && matching[i] == -1) {
                dfs(i, true);
            }
    }
}

bool checkAssignment(long long p, vector<vector<long long> > &c) {
    int n = c.size();
    g.clear();
    g.resize(2 * n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (p <= c[i][j]) {
                g[i].push_back(n + j);
                int temp = g[i].back();
                g[n + j].push_back(i);
            }
        }

//    for (int i = 0; i < 2 * n; i++) {
//        for (auto it = g[i].begin(); it != g[i].end(); it++)
//            cout << *it << " ";
//        cout << "\n";
//    }
//    cout << "\n";

    if (p == 6) {
        int x;
        x = 5;
    }

    makeMatching();

    for (int i = 0; i < matching.size(); i++)
        if (matching[i] == -1)
            return false;

    return true;
}

long long getProductivity(vector<int> &a, vector<vector<long long> > &c) {
    long long ans = INT64_MAX;
    for (int i = 0; i < a.size(); i++) {
        ans = min(ans, c[i][a[i]]);
    }
    return ans;
}

pair<long long, vector<int> > naiveSolve(int n, vector<vector<long long> > &c) {
    vector<int> curr(n);
    for (int i = 0; i < n; i++)
        curr[i] = i;

    pair<long long, vector<int> > ans = make_pair(getProductivity(curr, c), curr);

    while (next_permutation(curr.begin(), curr.end())) {
        long long currProductivity = getProductivity(curr, c);

        if (currProductivity > ans.first)
            ans = make_pair(currProductivity, curr);
    }

    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    time_t inittime = clock();
    used.clear();
    g.clear();
    matching.clear();
    layers.clear();

    int n;
    cin >> n;
    vector<vector<long long> > c(n, vector<long long>(n, 0));
    vector<long long> costs(n * n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> c[i][j];
            costs[i * n + j] = c[i][j];
        }
    }

    sort(costs.begin(), costs.end());
    long long l = 0, r = n * n;

    bool tr = checkAssignment(costs[l], c);

    while (l < r - 1) {
        long long m = l + ((r - l) >> 1);
        tr = checkAssignment(costs[m], c);
        if (tr)
            l = m;
        else
            r = m;
    }

    tr = checkAssignment(costs[l], c);

    vector<int> oldMatching = matching;
    //minimize(n);

    cout << costs[l] << "\n";
    for (int i = 0; i < n - 1; i++)
        cout << matching[i] + 1 << " ";
    cout << matching[n - 1] + 1 << "\n";
    cout << 1. * (clock() - inittime) / CLOCKS_PER_SEC << "\n";

    return 0;
}