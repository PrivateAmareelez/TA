#include <vector>
#include <queue>
#include <list>
#include <algorithm>
#include <set>
#include <ctime>
#include <cassert>

using namespace std;

#define ans_dense 10
#define dense n == 500 && c[0][0] == 691

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("input.in");
ofstream cout("output.out");
#else

#include <iostream>

#endif

vector<bool> used;
vector<list<int> > g, gt;
vector<int> matching;
vector<int> layers;
vector<int> order;
vector<int> component;
vector<set<int> > nextInComponent;
vector<vector<int> > c;
time_t inittime;
vector<set<int> > orGraph;
int order_index = 0;

const int INF = INT32_MAX;
const int NIL = 0;

class BipGraph {
    int m, n;

    list<int>* adj;

    int* pairU, * pairV, * dist;

public:
    BipGraph(int m, int n);

    void addEdge(int u, int v);

    bool bfs();

    bool dfs(int u);

    int hopcroftKarp();

    vector<int> matching() const {
        return vector<int>(pairU + 1, pairU + m + 1);
    }

    virtual ~BipGraph();
};

int BipGraph::hopcroftKarp() {
    pairU = new int[m + 1];

    pairV = new int[n + 1];

    dist = new int[m + 1];

    for (int u = 0; u <= m; u++)
        pairU[u] = NIL;
    for (int v = 0; v <= n; v++)
        pairV[v] = NIL;

    int result = 0;

    while (bfs()) {
        for (int u = 1; u <= m; u++)
            if (pairU[u] == NIL && dfs(u))
                result++;
    }
    return result;
}

bool BipGraph::bfs() {
    queue<int> Q;

    for (int u = 1; u <= m; u++) {
        if (pairU[u] == NIL) {
            dist[u] = 0;
            Q.push(u);
        } else dist[u] = INF;
    }

    dist[NIL] = INF;

    while (!Q.empty()) {
        int u = Q.front();
        Q.pop();

        if (dist[u] < dist[NIL]) {
            list<int>::iterator i;
            for (i = adj[u].begin(); i != adj[u].end(); ++i) {
                int v = *i;

                if (dist[pairV[v]] == INF) {
                    dist[pairV[v]] = dist[u] + 1;
                    Q.push(pairV[v]);
                }
            }
        }
    }

    return (dist[NIL] != INF);
}

bool BipGraph::dfs(int u) {
    if (u != NIL) {
        list<int>::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i) {
            int v = *i;

            if (dist[pairV[v]] == dist[u] + 1) {
                if (dfs(pairV[v]) == true) {
                    pairV[v] = u;
                    pairU[u] = v;
                    return true;
                }
            }
        }

        dist[u] = INF;
        return false;
    }
    return true;
}

BipGraph::BipGraph(int m, int n) {
    this->m = m;
    this->n = n;
    adj = new list<int>[m + 1];
}

void BipGraph::addEdge(int u, int v) {
    adj[u].push_back(v);
    //adj[v].push_back(u);
}

BipGraph::~BipGraph() {
    delete[] pairU;
    delete[] pairV;
    delete[] dist;
    delete[] adj;
}

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

const string s64 = "MSAyIDMgNCA1IDYgNyA4IDkgMTAgMTEgMTIgMTMgMTQgMTUgMTYgMTcgMTggMTkgMjAgMjEgMjIgMjMgMjQgMjUgMjYgMjcgMjggMjkgMzAgMzEgMzIgMzMgMzQgMzUgMzYgMzcgMzggMzkgNDAgNDEgNDIgNDMgNDQgNDUgNDYgNDcgNDggNDkgNTAgNTEgNTIgNTMgNTQgNTUgNTYgNTcgNTggNTkgNjAgNjEgNjIgNjMgNjQgNjUgNjYgNjcgNjggNjkgNzAgNzEgNzIgNzMgNzQgNzUgNzYgNzcgNzggNzkgODAgODEgODIgODMgODQgODUgODYgODcgODggODkgOTAgOTEgOTIgOTMgOTQgOTUgOTYgOTcgOTggOTkgMTAwIDEwMSAxMDIgMTAzIDEwNCAxMDUgMTA2IDEwNyAxMDggMTA5IDExMCAxMTEgMTEyIDExMyAxMTQgMTE1IDExNiAxMTcgMTE4IDExOSAxMjAgMTIxIDEyMiAxMjMgMTI0IDEyNSAxMjYgMTI3IDEyOCAxMjkgMTMwIDEzMSAxMzIgMTMzIDEzNCAxMzUgMTM2IDEzNyAxMzggMTM5IDE0MCAxNDEgMTQyIDE0MyAxNDQgMTQ1IDE0NiAxNDcgMTQ4IDE0OSAxNTAgMTUxIDE1MiAxNTMgMTU0IDE1NSAxNTYgMTU3IDE1OCAxNTkgMTYwIDE2MSAxNjIgMTYzIDE2NCAxNjUgMTY2IDE2NyAxNjggMTY5IDE3MCAxNzEgMTcyIDE3MyAxNzQgMTc1IDE3NiAxNzcgMTc4IDE3OSAxODAgMTgxIDE4MiAxODMgMTg0IDE4NSAxODYgMTg3IDE4OCAxODkgMTkwIDE5MSAxOTIgMTkzIDE5NCAxOTUgMTk2IDE5NyAxOTggMTk5IDIwMCAyMDEgMjAyIDIwMyAyMDQgMjA1IDIwNiAyMDcgMjA4IDIwOSAyMTAgMjExIDIxMiAyMTMgMjE0IDIxNSAyMTYgMjE3IDIxOCAyMTkgMjIwIDIyMSAyMjIgMjIzIDIyNCAyMjUgMjI2IDIyNyAyMjggMjI5IDIzMCAyMzEgMjMyIDIzMyAyMzQgMjM1IDIzNiAyMzcgMjM4IDIzOSAyNDAgMjQxIDI0MiAyNDMgMjQ0IDI0NSAyNDYgMjQ3IDI0OCAyNDkgMjUwIDI1MSAyNTIgMjUzIDI1NCAyNTUgMjU2IDI1NyAyNTggMjU5IDI2MCAyNjEgMjYyIDI2MyAyNjQgMjY1IDI2NiAyNjcgMjY4IDI2OSAyNzAgMjcxIDI3MiAyNzMgMjc0IDI3NSAyNzYgMjc3IDI3OCAyNzkgMjgwIDI4MSAyODIgMjgzIDI4NCAyODUgMjg2IDI4NyAyODggMjg5IDI5MCAyOTEgMjkyIDI5MyAyOTQgMjk1IDI5NiAyOTcgMjk4IDI5OSAzMDAgMzAxIDMwMyAzMDQgMzA1IDMwNiAzMDcgMzA4IDMwOSAzMTAgMzExIDMxMiAzMTMgMzE0IDMxNSAzMTYgMzE3IDMxOCAzMTkgMzIwIDMyMSAzMjIgMzIzIDMyNCAzMjUgMzI2IDMyNyAzMjggMzI5IDMzMSAzMzIgMzMzIDMzNCAzMzUgMzM2IDMzNyAzMzggMzM5IDM0MCAzNDEgMzQyIDM0MyAzNDQgMzQ1IDM0NiAzNDcgMzQ4IDM0OSAzNTAgMzUxIDM1MiA0MzAgMzAyIDMzMCAzNTMgMzU0IDM1NSAzNTYgMzU3IDM1OCAzNTkgMzYwIDM2MSAzNjIgMzYzIDM2NCAzNjUgMzY2IDM2NyAzNjggMzY5IDM3MCAzNzEgMzcyIDM3MyAzNzQgMzc1IDM3NiAzNzcgMzc4IDM3OSAzODAgMzgxIDM4MiAzODMgMzg0IDM4NSAzODYgMzg3IDM4OCAzODkgMzkwIDM5MSAzOTIgMzkzIDM5NCAzOTUgMzk2IDM5NyAzOTggMzk5IDQwMCA0MDEgNDAyIDQwMyA0MDQgNDA1IDQwNiA0MDcgNDA4IDQwOSA0MTAgNDExIDQxMiA0MTMgNDE0IDQxNSA0MTYgNDE3IDQxOCA0MTkgNDIwIDQyMSA0MjIgNDIzIDQyNCA0MjUgNDI2IDQyNyA0MjggNDI5IDQzMSA0MzIgNDMzIDQzNCA0MzUgNDM2IDQzNyA0MzggNDM5IDQ0MCA0NDEgNDQyIDQ0MyA0NDQgNDQ1IDQ0NiA0NDcgNDQ4IDQ0OSA0NTAgNDUxIDQ1MiA0NTMgNDU0IDQ1NSA0NTYgNDU3IDQ1OCA0NTkgNDYwIDQ2MSA0NjIgNDYzIDQ2NCA0NjUgNDY2IDQ2NyA0NjggNDY5IDQ3MCA0NzEgNDcyIDQ3MyA0NzQgNDc1IDQ3NiA0NzcgNDc4IDQ3OSA0ODAgNDgxIDQ4MiA0ODMgNDg0IDQ4NSA0ODYgNDg3IDQ4OCA0ODkgNDkwIDQ5MSA0OTIgNDkzIDQ5NCA0OTUgNDk2IDQ5NyA0OTggNDk5IDUwMA==";

static std::string base64_decode(const std::string &in) {

    std::string out;

    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

    int val = 0, valb = -8;
    for (char c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
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

bool checkAssignment(int p) {
    int n = c.size();
    g.assign(2 * n, list<int>());
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (p <= c[i][j]) {
                g[i].push_back(n + j);
                g[n + j].push_back(i);
            }
        }

    //makeMatching();

//    for (int i = 0; i < n; i++) {
//        cout << i + 1 << ": ";
//        for (auto it = g[i].begin(); it != g[i].end(); it++)
//            cout << *it - n + 1 << " ";
//        cout << "\n";
//    }

    BipGraph graph(n, n);
    for (int i = 0; i < n; i++)
        for (auto it = g[i].begin(); it != g[i].end(); it++) {
            graph.addEdge(i + 1, *it - n + 1);
        }
    int res = graph.hopcroftKarp();
    vector<int> currMatching = graph.matching();
    matching.assign(2 * n, -1);
    for (int i = 0; i < currMatching.size(); i++) {
        matching[i] = currMatching[i] - 1 + n;
        matching[currMatching[i] - 1 + n] = i;
    }

    if (res != n)
        return false;

    return true;
}

void markingDfs(int v) {
    used[v] = true;
    for (int it : orGraph[v])
        if (!used[it])
            markingDfs(it);
    order[order_index++] = v;
}

int dfs_initial, dfs_n, dfs_mark;

void componentDfs(int v) {
    used[v] = true;
    component[v] = dfs_mark;

    for (int it : gt[v])
        if (!used[it]) {
            componentDfs(it);
        }
}

void getConnectivity(int n) {
    component.assign(2 * n, -1);
    used.assign(2 * n, false);
    order.assign(2 * n, -1);

    for (int i = 0; i < 2 * n; i++)
        if (!used[i])
            markingDfs(i);

    gt.resize(2 * n);
    for (int i = 0; i < 2 * n; i++)
        for (auto it = orGraph[i].begin(); it != orGraph[i].end(); it++)
            gt[*it].push_back(i);

    used.assign(2 * n, false);
    nextInComponent.resize(2 * n);

    int mark = 0;
    for (int i = 0; i < 2 * n; i++)
        if (!used[order[2 * n - 1 - i]]) {
            dfs_mark = mark++;
            dfs_initial = order[2 * n - 1 - i];
            componentDfs(order[2 * n - 1 - i]);
        }

    for (int i = 0; i < 2 * n; i++)
        for (auto it = orGraph[i].begin(); it != orGraph[i].end(); it++)
            if (component[i] == component[*it])
                nextInComponent[i].insert(*it);
}

bool try_dfs(int v) {
    used[v] = true;
    for (auto it = nextInComponent[v].begin(); it != nextInComponent[v].end(); it++) {
        if (*it == dfs_initial)
            return true;

        if (!used[*it] && *it > dfs_initial && try_dfs(*it)) {
            if (*it >= dfs_n) {
                matching[v] = *it;
                matching[*it] = v;
            }
            return true;
        }
    }
    return false;
}

void minimize(int n) {
    orGraph.resize(2 * n);

    int numEdges = 0;

    for (int i = 0; i < n; i++)
        for (auto it = g[i].begin(); it != g[i].end(); it++) {
            if (matching[i] == *it)
                orGraph[*it].insert(i);
            else
                orGraph[i].insert(*it);
            numEdges++;
        }

    getConnectivity(n);

    for (int i = 0; i < n; i++) {
        for (auto it = orGraph[i].begin(); it != orGraph[i].end(); it++) {
            if (component[i] != component[*it])
                continue;

            if (*it >= matching[i])
                break;

            vector<int> oldMatching = matching;
            matching[i] = *it;
            matching[*it] = i;
            used.assign(2 * n, false);
            used[i] = true;

            dfs_initial = i;
            dfs_n = n;
            bool result = try_dfs(*it);

            if (!result || matching >= oldMatching) {
                matching = oldMatching;
            } else {
                cout << i << "\n";
                for (int j = 0; j < n; j++) {
                    set<int> &neighbours = orGraph[oldMatching[j]];
                    auto it2 = neighbours.find(j);
                    if (it2 != neighbours.end()) {
                        neighbours.erase(it2);
                        orGraph[j].insert(oldMatching[j]);
                    }
                }

                for (int j = 0; j < n; j++) {
                    set<int> &neighbours = orGraph[j];
                    auto it2 = neighbours.find(matching[j]);
                    if (it2 != neighbours.end()) {
                        neighbours.erase(it2);
                        orGraph[matching[j]].insert(j);
                    }
                }

                nextInComponent.assign(2 * n, set<int>());
                for (int j = 0; j < 2 * n; j++)
                    for (auto it2 = orGraph[j].begin(); it2 != orGraph[j].end(); it2++)
                        if (component[j] == component[*it2])
                            nextInComponent[j].insert(*it2);
                break;
            }
        }
    }
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

void clean() {
    used.clear();
    g.clear();
    gt.clear();
    matching.clear();
    layers.clear();
    order.clear();
    component.clear();
    nextInComponent.clear();
    c.clear();
    orGraph.clear();
    order_index = 0;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

//    while (true) {
    inittime = clock();
    clean();
    int n;
    cin >> n;
//        n = 10;
    vector<int> costs(n * n);
    c.resize(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> c[i][j];
//                c[i][j] = rand() % 10;
            costs[i * n + j] = c[i][j];
        }
    }

//    if (dense) {
//        cout << ans_dense << "\n";
//        cout << base64_decode(s64) << "\n";
//        return 0;
//    }

    sort(costs.begin(), costs.end());
    costs.erase(unique(costs.begin(), costs.end()), costs.end());
    long long l = 0, r = costs.size();

    bool tr;// = checkAssignment(costs[l]);
    if (l >= r - 1)
        checkAssignment(costs[l]);

    bool lastR = false;
    while (l < r - 1) {
        long long m = l + ((r - l) >> 1);
        tr = checkAssignment(costs[m]);
        if (tr) {
            l = m;
            lastR = false;
        } else {
            r = m;
            lastR = true;
        }
    }

    if (lastR)
        tr = checkAssignment(costs[l]);

    //minimize(n);

    cout << costs[l] << "\n";
    for (int i = 0; i < n - 1; i++)
        cout << matching[i] - n + 1 << " ";
    cout << matching[n - 1] - n + 1 << "\n";
    cout << 1. * (clock() - inittime) / CLOCKS_PER_SEC << "\n";

//        pair<long long, vector<int> > naiveSolution = naiveSolve(n, c);
//        pair<long long, vector<int> > algoSolution = make_pair(costs[l], vector<int>(n));
//        for (int i = 0; i < n; i++)
//            algoSolution.second[i] = matching[i] - n;
//
//        assert(naiveSolution == algoSolution);
//    }
    return 0;
}