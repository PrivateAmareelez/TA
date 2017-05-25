#include <vector>
#include <queue>
#include <list>
#include <algorithm>
#include <assert.h>
#include <set>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("input.in");
ofstream cout("output.out");
#else

#include <iostream>

#endif

vector<bool> used;
vector<set<int> > g, gt;
vector<int> matching;
vector<int> layers;
vector<int> order;
vector<int> component;
vector<set<int> > nextInComponent;

bool dfs(int v, bool edgeInMatching) {
    used[v] = true;

    if (matching[v] == -1 && v < matching.size() / 2)
        return true;

    for (auto it = g[v].begin(); it != g[v].end(); it++) {
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

            for (auto it = g[curr].begin(); it != g[curr].end(); it++) {
                if (curr < n) {
                    if (matching[curr] != *it && !used[*it]) {
                        q.push(*it);
                        used[*it] = true;
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

bool checkAssignment(int p, vector<vector<int> > &c) {
    int n = c.size();
    g.clear();
    g.resize(2 * n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (p <= c[i][j]) {
                g[i].insert(n + j);
                //int temp = g[i].back();
                g[n + j].insert(i);
            }
        }

//    for (int i = 0; i < 2 * n; i++) {
//        for (auto it = g[i].begin(); it != g[i].end(); it++)
//            cout << *it << " ";
//        cout << "\n";
//    }
//    cout << "\n";

    makeMatching();

    for (int i = 0; i < matching.size(); i++)
        if (matching[i] == -1)
            return false;

    return true;
}

long long getProductivity(vector<int> &a, vector<vector<int> > &c) {
    int ans = INT32_MAX;
    for (int i = 0; i < a.size(); i++) {
        ans = min(ans, c[i][a[i]]);
    }
    return ans;
}

pair<long long, vector<int> > naiveSolve(int n, vector<vector<int> > &c) {
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

void markingDfs(int v) {
    used[v] = true;
    for (auto it = g[v].begin(); it != g[v].end(); it++)
        if (!used[*it])
            markingDfs(*it);
    order.push_back(v);
}

void componentDfs(int v, int mark, int initial) {
    used[v] = true;
    component[v] = mark;

    bool has = false;
    for (auto it = gt[v].begin(); it != gt[v].end(); it++)
        if (!used[*it]) {
            //nextInComponent[*it].insert(v);
            has = true;
            componentDfs(*it, mark, initial);
        }
//
//    if (!has)
//        nextInComponent[initial].insert(v);
}

void getConnectivity(int n) {
    component.resize(2 * n, -1);
    used.clear();
    used.resize(2 * n, false);
    order.clear();
    order.reserve(2 * n);

    for (int i = 0; i < 2 * n; i++)
        if (!used[i])
            markingDfs(i);

    gt.clear();
    gt.resize(2 * n);
    for (int i = 0; i < 2 * n; i++)
        for (auto it = g[i].begin(); it != g[i].end(); it++)
            gt[*it].insert(i);

//    for (int i = 0; i < n; i++) {
//        cout << i << ": ";
//        for (auto it = gt[i].begin(); it != gt[i].end(); it++)
//            cout << *it << " ";
//        cout << "\n";
//    }
//    cout << "\n";

    used.clear();
    used.resize(2 * n, false);
    nextInComponent.clear();
    nextInComponent.resize(2 * n);

    int mark = 0;
    for (int i = 0; i < 2 * n; i++)
        if (!used[order[2 * n - 1 - i]])
            componentDfs(order[2 * n - 1 - i], mark++, order[2 * n - 1 - i]);

    for (int i = 0; i < 2 * n; i++)
        for (auto it = g[i].begin(); it != g[i].end(); it++)
            if (component[i] == component[*it])
                nextInComponent[i].insert(*it);
}

bool try_dfs(int v, int initial, int n) {
    used[v] = true;
    for (auto it = nextInComponent[v].begin(); it != nextInComponent[v].end(); it++) {
        if (*it == initial)
            return true;

        if (!used[*it] && *it > initial && try_dfs(*it, initial, n)) {
            if (*it >= n) {
                matching[v] = *it;
                matching[*it] = v;
            }
            return true;
        }
    }
    return false;
}

void minimize(int n) {
    vector<set<int> > temp(2 * n);
    for (int i = n; i < 2 * n; i++)
        g[i].clear();
    for (int i = 0; i < n; i++)
        for (auto it = g[i].begin(); it != g[i].end(); it++)
            if (matching[i] == *it)
                temp[*it].insert(i);
            else
                temp[i].insert(*it);
    g = temp;

    getConnectivity(n);

//    for (int i = 0; i < 2 * n; i++)
//        cout << component[i] << " ";
//    cout << "\n";

//    for (int i = 0; i < 2 * n; i++) {
//        cout << i << ": ";
//        for (auto it = nextInComponent[i].begin(); it != nextInComponent[i].end(); it++)
//            cout << *it << " ";
//        cout << "\n";
//    }
//    cout << "\n";

    for (int i = 0; i < n; i++) {
//        auto l = g[i].begin();
//        for (int i = 0; i < g[i].size(); i++)
//            cout << *(l++) << " ";
//        cout << "\n";

        for (auto it = g[i].begin(); it != g[i].end(); it++) {
            int temp3 = *it;
            if (component[i] != component[*it])
                continue;

            if (*it >= matching[i])
                break;

            vector<int> oldMatching = matching;
            int v = *it;
            matching[i] = *it;
            matching[*it] = i;
            used.clear();
            used.resize(2 * n, false);
            used[i] = true;
            bool result = try_dfs(*it, i, n);
            //vector<int> newMatching = matching;

            if (!result || matching >= oldMatching) {
                matching = oldMatching;
            } else {
                for (int j = 0; j < n; j++) {
                    for (auto it2 = g[oldMatching[j]].begin(); it2 != g[oldMatching[j]].end(); it2++)
                        if (*it2 == j) {
                            int temp2 = *it2;
                            g[oldMatching[j]].erase(it2);
                            g[j].insert(oldMatching[j]);
                            break;
                        }
                }

                for (int j = 0; j < n; j++) {
                    for (auto it2 = g[j].begin(); it2 != g[j].end(); it2++)
                        if (*it2 == matching[j]) {
                            int temp2 = *it2;
                            g[j].erase(it2);
                            g[matching[j]].insert(j);
                            break;
                        }
                }

                nextInComponent.clear();
                nextInComponent.resize(2 * n);
                for (int j = 0; j < 2 * n; j++)
                    for (auto it = g[j].begin(); it != g[j].end(); it++)
                        if (component[j] == component[*it])
                            nextInComponent[j].insert(*it);
//                for (int j = 0; j < 2 * n; j++) {
//                    cout << j << ": ";
//                    for (auto it = nextInComponent[j].begin(); it != nextInComponent[j].end(); it++)
//                        cout << *it << " ";
//                    cout << "\n";
//                }
//                cout << "\n";

                break;
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    time_t inittime = clock();
    int n;
    cin >> n;
    //n = 500;
    vector<vector<int> > c(n, vector<int>(n));
    vector<int> costs(n * n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> c[i][j];
            //c[i][j] = rand() % 10;
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

    //vector<int> oldMatching = matching;

//        for (int i = 0; i < n; i++) {
//            cout << i << ": ";
//            for (list<int>::iterator it = g[i].begin(); it != g[i].end(); it++)
//                cout << *it << " ";
//            cout << "\n";
//        }
//        cout << "\n";

    minimize(n);
//        vector<int> oldMatching = matching;
//        bool smthChanged = true;
//        while(smthChanged) {
//            smthChanged = false;
//            for (int i = 0; i < n; i++) {
//                for (int j = i + 1; j < n; j++) {
//                    int m1 = matching[i], m2 = matching[j];
//                    if (matching[j] < matching[i] && graph[i][m2] && graph[j][m1]) {
//                        swap(matching[i], matching[j]);
//                        smthChanged = true;
//                        break;
//                    }
//                }
//            }
//        }

    //cout << "------------------------------\niter: " << cnt << "\nmatching:\n";
    cout << costs[l] << "\n";
    for (int i = 0; i < n - 1; i++)
        cout << matching[i] - n + 1 << " ";
    cout << matching[n - 1] - n + 1 << "\n";
    cout << 1. * (clock() - inittime) / CLOCKS_PER_SEC << "\n";

//        cout << "old matching:\n" << costs[l] << "\n";
//        for (int i = 0; i < n - 1; i++)
//            cout << oldMatching[i] - n + 1 << " ";
//        cout << oldMatching[n - 1] - n + 1 << "\n";

//        pair<long long, vector<int> > naiveSolution = naiveSolve(n, c);
//        pair<long long, vector<int> > algoSolution = make_pair(costs[l], vector<int>(n));
//        for (int i = 0; i < n; i++)
//            algoSolution.second[i] = matching[i] - n;
//
//        assert(matching <= oldMatching);
//        assert(naiveSolution == algoSolution);

    return 0;
}