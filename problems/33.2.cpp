#include <vector>
#include <ctime>
#include <stack>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("input.txt");
ofstream cout("output.txt");
#else
#include <iostream>
#endif

#define StartClock time_t inittime=clock();
#define GetClock fprintf(stderr,"Time: %f\n",1.0*(clock()-inittime)/CLOCKS_PER_SEC);

//---------------DP--------------
const long long INF = INT64_MAX / 100;

vector<vector<long long> > firstDP;
vector<pair<long long, long long> > coord;
int n;

long long getDist(int i, int j) {
    return abs(coord[i].first - coord[j].first) + abs(coord[i].second - coord[j].second);
}

bool getBit(long long mask, int i) {
    return mask & (1 << i);
}

int getCardinality(long long mask) {
    int ans = 0;
    while (mask) {
        ans += mask & 1;
        mask >>= 1;
    }
    return ans;
}

long long getCycleLength(int i, long long mask) {
    //cout << i << " " << mask << "\n";
    if (firstDP[i][mask] != INF)
        return firstDP[i][mask];

    for (int j = 0; j < n; j++) {
        if (getBit(mask, j)) {
            firstDP[i][mask] = min(firstDP[i][mask], getCycleLength(j, mask ^ (1 << j)) + getDist(i, j));
        }
    }

    return firstDP[i][mask];
}

vector<int> getCycle(const long long maskSize) {
    stack<int> cycle;
    int i = 0;
    long long mask = maskSize - 1;
    cycle.push(i);

    while (mask) {
        for (int j = 0; j < n; j++) {
            if (getBit(mask, j) && firstDP[i][mask] == firstDP[j][mask ^ (1 << j)] + getDist(i, j)) {
                cycle.push(j);
                i = j;
                mask ^= 1 << j;
            }
        }
    }

    vector<int> path(cycle.size());
    i = 0;
    while (!cycle.empty()) {
        path[i++] = cycle.top();
        cycle.pop();
    }

    return path;
}

vector<int> getCycle2(const long long maskSize) {
    stack<int> cycle;
    int i = 0;
    long long mask = maskSize - 1;
    cycle.push(i);

    while (mask) {
        for (int j = 0; j < n; j++) {
            long long x = firstDP[i][mask], y = firstDP[j][mask ^ (1 << i)];
            if (getBit(mask, j) && firstDP[i][mask] == firstDP[j][mask ^ (1 << i)] + getDist(i, j)) {
                cycle.push(j);
                mask ^= 1 << i;
                i = j;
            }
        }
    }

    cycle.pop();
    cycle.push(0);
    vector<int> path(cycle.size());
    i = 0;
    while (!cycle.empty()) {
        path[i++] = cycle.top();
        cycle.pop();
    }

    return path;
}

//--------------Branch----------------
int N;
time_t inittime;

vector<int> final_path;

vector<bool> visited;

double final_res = INT64_MAX;

void copyToFinal(vector<int> &curr_path) {
    for (int i = 0; i < N; i++)
        final_path[i] = curr_path[i];
    final_path[N] = curr_path[0];
}

double firstMin(vector<vector<double> > &adj, int i) {
    double min = INT64_MAX;
    for (int k = 0; k < N; k++)
        if (adj[i][k] < min && i != k)
            min = adj[i][k];
    return min;
}

double secondMin(vector<vector<double> > &adj, int i) {
    double first = INT64_MAX, second = INT64_MAX;
    for (int j = 0; j < N; j++) {
        if (i == j)
            continue;

        if (adj[i][j] <= first) {
            second = first;
            first = adj[i][j];
        } else if (adj[i][j] <= second &&
                   adj[i][j] != first)
            second = adj[i][j];
    }
    return second;
}

void TSPRec(vector<vector<double> > &adj, double curr_bound, double curr_weight, int level,
            vector<int> &curr_path) {
    if (1.0 * (clock() - inittime) / CLOCKS_PER_SEC >= 0.95)
        return;

    if (level == N) {
        if (adj[curr_path[level - 1]][curr_path[0]] != 0) {
            double curr_res = curr_weight + adj[curr_path[level - 1]][curr_path[0]];

            if (curr_res < final_res) {
                copyToFinal(curr_path);
                final_res = curr_res;
            }
        }
        return;
    }

    for (int i = 0; i < N; i++) {
        if (adj[curr_path[level - 1]][i] != 0 && visited[i] == false) {
            double temp = curr_bound;
            curr_weight += adj[curr_path[level - 1]][i];

            if (level == 1)
                curr_bound -= ((firstMin(adj, curr_path[level - 1]) +
                                firstMin(adj, i)) / 2);
            else
                curr_bound -= ((secondMin(adj, curr_path[level - 1]) +
                                firstMin(adj, i)) / 2);

            if (curr_bound + curr_weight < final_res) {
                curr_path[level] = i;
                visited[i] = true;

                if (1.0 * (clock() - inittime) / CLOCKS_PER_SEC < 0.95)
                    TSPRec(adj, curr_bound, curr_weight, level + 1,
                           curr_path);
            }

            curr_weight -= adj[curr_path[level - 1]][i];
            curr_bound = temp;

            visited.clear();
            visited.resize(N + 1, false);
            for (int j = 0; j <= level - 1; j++)
                visited[curr_path[j]] = true;
        }
    }
}

void TSP(vector<vector<double> > &adj) {
    vector<int> curr_path(N + 1, -1);

    double curr_bound = 0;
    visited.resize(N + 1, false);

    for (int i = 0; i < N; i++)
        curr_bound += (firstMin(adj, i) +
                       secondMin(adj, i));

    //curr_bound = (curr_bound & 1) ? (curr_bound / 2 + 1) : (curr_bound / 2);
    curr_bound /= 2;

    visited[0] = true;
    curr_path[0] = 0;

    TSPRec(adj, curr_bound, 0, 1, curr_path);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    inittime = clock();

    cin >> N;
    vector<pair<long long, long long> > v(N);
    for (int i = 0; i < N; i++)
        cin >> v[i].first >> v[i].second;

    if (N <= 12) {
        n = N;
        coord = v;

        long long maskSize = 1;
        for (int i = 0; i < n; i++)
            maskSize *= 2;

        firstDP.resize(n, vector<long long>(maskSize, INF));
        firstDP[0][0] = 0;

        long long ans = getCycleLength(0, maskSize - 1);
        cout << ans << "\n";

        vector<int> path = getCycle(maskSize);
        for (int i = 0; i < path.size() - 1; i++)
            cout << path[i] + 1 << " ";
        cout << path[path.size() - 1] + 1 << "\n";

        ////////////////

        maskSize = 1;
        for (int i = 0; i < n; i++)
            maskSize *= 2;

        firstDP.clear();
        firstDP.resize(n, vector<long long>(maskSize, INF));
        for (int i = 0; i < n; i++) {
            firstDP[i][1 << i] = 0;
            firstDP[i][0] = 0;
        }

        for (int i = 2; i <= n; i++) {
            vector<int> bits(n, 0);
            for (int j = 0; j < i; j++)
                bits[j] = 1;

            long long mask = 0;
            for (int j = 0; j < n; j++)
                if (bits[j] == 1)
                    mask |= 1 << j;

            for (int j = 0; j < n; j++) {
                if (getBit(mask, j)) {
                    long long temp = mask ^(1 << j);
                    long long x = firstDP[j][mask];
                    for (int k = 0; k < n; k++)
                        if (getBit(temp, k)) {
                            firstDP[j][mask] = min(firstDP[j][mask], firstDP[k][temp] + getDist(k, j));
                            x = firstDP[j][mask];
                        }
                }
            }

            while (prev_permutation(bits.begin(), bits.end())) {
                mask = 0;
                for (int j = 0; j < n; j++)
                    if (bits[j] == 1)
                        mask |= 1 << j;

                for (int j = 0; j < n; j++) {
                    if (getBit(mask, j)) {
                        long long temp = mask ^(1 << j);
                        for (int k = 0; k < n; k++)
                            if (getBit(temp, k))
                                firstDP[j][mask] = min(firstDP[j][mask], firstDP[k][temp] + getDist(k, j));
                    }
                }
            }
        }

        long long ans2 = firstDP[0][maskSize - 1];
        vector<int> path2 = getCycle2(maskSize);
        cout << ans2 + getDist(0, path2[1]) << "\n";
        for (int i = 0; i < path2.size() - 1; i++)
            cout << path2[i] + 1 << " ";
        cout << path2[path2.size() - 1] + 1 << "\n";
    } else {
        vector<vector<double> > adj(N, vector<double>(N));
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                adj[i][j] = abs(v[i].first - v[j].first) + abs(v[i].second - v[j].second);
            }
        }

        final_path.resize(N);

        TSP(adj);

        cout << final_res << "\n";
        for (int i = 0; i < N; i++)
            cout << final_path[i] + 1 << " ";
        cout << final_path[N] + 1 << "\n";
    }
    return 0;
}