#include <vector>
#include <ctime>
#include <stack>
#include <algorithm>

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
vector<pair<long long, long long> > coord;
vector<vector<long long> > firstDP;
vector<vector<int> > secondDP;
int n;

inline long long getDist(int i, int j) {
    return abs(coord[i].first - coord[j].first) + abs(coord[i].second - coord[j].second);
}

inline bool getBit(long long mask, int i) {
    return mask & (1 << i);
}

void solveDP() {
    if (n == 1) {
        cout << "0\n";
        cout << "1 1\n";
        return;
    }

    long long maskSize = 1;
    for (int i = 0; i < n; i++)
        maskSize *= 2;

    firstDP.resize(n, vector<long long>(maskSize, INF));
    secondDP.resize(n, vector<int>(maskSize, -1));

    for (int i = 0; i < n; i++) {
        firstDP[i][1 << i] = getDist(0, i);
    }

    for (int i = 2; i <= n - 1; i++) {
        vector<int> bits(n, 0);
        for (int j = 0; j < i; j++)
            bits[j] = 1;

        long long mask = 0;
        for (int j = 0; j < n; j++)
            if (bits[j] == 1)
                mask |= 1 << j;

        if ((mask & 1) == 0) {
            for (int j = 0; j < n; j++) {
                if (getBit(mask, j)) {
                    for (int k = 0; k < n; k++) {
                        if (getBit(mask ^ (1 << j), k)) {
                            long long dist = firstDP[k][mask ^ (1 << j)] + getDist(k, j);
                            if (firstDP[j][mask] > dist) {
                                firstDP[j][mask] = dist;
                                secondDP[j][mask] = k;
                            }
                        }
                    }
                }
            }
        }

        while (prev_permutation(bits.begin(), bits.end())) {
            mask = 0;
            for (int j = 0; j < n; j++)
                if (bits[j] == 1)
                    mask |= 1 << j;

            if (mask & 1)
                continue;

            for (int j = 0; j < n; j++) {
                if (getBit(mask, j)) {
                    for (int k = 0; k < n; k++) {
                        if (getBit(mask ^ (1 << j), k)) {
                            long long dist = firstDP[k][mask ^ (1 << j)] + getDist(k, j);
                            if (firstDP[j][mask] > dist) {
                                firstDP[j][mask] = dist;
                                secondDP[j][mask] = k;
                            }
                        }
                    }
                }
            }
        }
    }

    long long ans = INF;
    for (int i = 1; i < n; i++) {
        long long dist = firstDP[i][(maskSize - 1) ^ 1] + getDist(i, 0);
        if (ans > dist) {
            ans = dist;
            secondDP[0][(maskSize - 1)] = i;
        }
    }

    vector<int> path;
    int p = 0;
    long long temp = (maskSize - 1);
    path.push_back(p);
    while (secondDP[p][temp] != -1) {
        int curr = p;
        p = secondDP[p][temp];
        temp ^= 1 << curr;
        path.push_back(p);
    }
    path.push_back(0);
    cout << ans << "\n";
    for (int i = 0; i < path.size() - 1; i++)
        cout << path[i] + 1 << " ";
    cout << path[path.size() - 1] + 1 << "\n";
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

double getLowerBoundMinMin(int level, int i, vector<vector<double> > &adj, vector<int> &curr_path, double curr_bound) {
    if (level == 1)
        curr_bound -= ((firstMin(adj, curr_path[level - 1]) + firstMin(adj, i)) / 2);
    else
        curr_bound -= ((secondMin(adj, curr_path[level - 1]) + firstMin(adj, i)) / 2);
    return curr_bound;
}

double getLowerBoundDp(vector<int> &curr_path, int level, int vertex) {
    long long maskSize = 1;
    for (int i = 0; i < n; i++)
        maskSize *= 2;

    firstDP.clear();
    secondDP.clear();
    firstDP.resize(n, vector<long long>(maskSize, INF));
    secondDP.resize(n, vector<int>(maskSize, -1));

    long long left = 0;
    for (int i = 0; i < level; i++)
        left |= 1 << curr_path[i];
    left |= 1 << vertex;

    left ^= maskSize - 1;
    //00011 - left
    //11111 - maskSize
    //11100
    for (int i = 0; i < n; i++) {
        if (getBit(left, i))
            firstDP[i][1 << i] = getDist(vertex, i);
    }

    if (level == n - 1) {
        return getDist(vertex, 0);
    }

    for (int i = 2; i <= n - level - 1; i++) {
        vector<int> bits(n, 0);
        for (int j = 0; j < i; j++)
            bits[j] = 1;

        long long mask = 0;
        for (int j = 0; j < n; j++) {
            if (bits[j] == 1)
                mask |= 1 << j;
        }

        if ((mask == (mask & left)) && (mask & (1 << vertex)) == 0) {
            for (int j = 0; j < n; j++) {
                if (getBit(mask, j)) {
                    for (int k = 0; k < n; k++) {
                        if (getBit(mask ^ (1 << j), k) && getBit(left, k)) {
                            long long dist = firstDP[k][mask ^ (1 << j)] + getDist(k, j);
                            if (firstDP[j][mask] > dist) {
                                firstDP[j][mask] = dist;
                                secondDP[j][mask] = k;
                            }
                        }
                    }
                }
            }
        }

        while (prev_permutation(bits.begin(), bits.end())) {
            mask = 0;

            for (int j = 0; j < n; j++)
                if (bits[j] == 1)
                    mask |= 1 << j;

            if ((mask & (1 << vertex)) || mask != (mask & left))
                continue;

            for (int j = 0; j < n; j++) {
                if (getBit(mask, j)) {
                    for (int k = 0; k < n; k++) {
                        if (getBit(mask ^ (1 << j), k) && getBit(left, k)) {
                            long long dist = firstDP[k][mask ^ (1 << j)] + getDist(k, j);
                            if (firstDP[j][mask] > dist) {
                                firstDP[j][mask] = dist;
                                secondDP[j][mask] = k;
                            }
                        }
                    }
                }
            }
        }
    }

    long long ans = INF;
    for (int i = 1; i < n; i++) {
        if (!getBit(left, i))
            continue;

        long long dist = firstDP[i][left] + getDist(i, 0);
        if (ans > dist) {
            ans = dist;
            secondDP[0][left] = i;
        }
    }

    return ans;
}

void TSPRec(vector<vector<double> > &adj, double curr_bound, double curr_weight, int level, vector<int> &curr_path) {
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
        if (curr_path[0] == 0 && curr_path[1] == 3 && i == 12 && level == 2) {
            int x;
            x = 5;
        }

        if (adj[curr_path[level - 1]][i] != 0 && visited[i] == false) {
            double temp = curr_bound;
            curr_weight += adj[curr_path[level - 1]][i];

            curr_bound = getLowerBoundMinMin(level, i, adj, curr_path, curr_bound);
            double curr_bound_dp = INF;

            if (1.0 * (clock() - inittime) / CLOCKS_PER_SEC >= 0.95)
                return;

            if (N - level - 1 <= 16) {
                curr_bound_dp = getLowerBoundDp(curr_path, level, i);
                curr_bound = curr_bound_dp;
            }
//            if (curr_bound_dp != INF && curr_bound_dp < curr_bound)
//                cout << curr_bound_dp << " " << curr_bound << "\n";

            if (curr_bound + curr_weight < final_res) {
                curr_path[level] = i;
                visited[i] = true;

                if (1.0 * (clock() - inittime) / CLOCKS_PER_SEC < 0.95)
                    TSPRec(adj, curr_bound, curr_weight, level + 1, curr_path);
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
    coord.resize(N);
    for (int i = 0; i < N; i++)
        cin >> coord[i].first >> coord[i].second;

    n = N;
    if (N <= 17) {
        solveDP();
    } else {
        vector<vector<double> > adj(N, vector<double>(N));
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                adj[i][j] = abs(coord[i].first - coord[j].first) + abs(coord[i].second - coord[j].second);
            }
        }

        final_path.resize(N + 1);

        TSP(adj);

        cout << final_res << "\n";
        for (int i = 0; i < N; i++)
            cout << final_path[i] + 1 << " ";
        cout << final_path[N] + 1 << "\n";
    }
    cout << 1. * (clock() - inittime) / CLOCKS_PER_SEC << "\n";
    return 0;
}