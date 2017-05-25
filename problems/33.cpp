#include <vector>
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

const long long INF = INT64_MAX / 100;
vector<pair<long long, long long> > coord;
int n;

inline long long getDist(int i, int j) {
    return abs(coord[i].first - coord[j].first) + abs(coord[i].second - coord[j].second);
}

inline bool getBit(long long mask, int i) {
    return mask & (1 << i);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> n;

    coord.resize(n);
    for (int i = 0; i < n; i++)
        cin >> coord[i].first >> coord[i].second;

    if (n == 1) {
        cout << "0\n";
        cout << "1 1\n";
        return 0;
    }

    long long maskSize = 1;
    for (int i = 0; i < n; i++)
        maskSize *= 2;

    vector<vector<long long> > firstDP(n, vector<long long>(maskSize, INF));
    vector<vector<int> > secondDP(n, vector<int>(maskSize, -1));
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
            for (int j = 0; j < n; j++)
                if (bits[j] == 1)
                    mask |= 1 << j;

            for (int j = 0; j < n; j++) {
                if (getBit(mask, j)) {
                    for (int k = 0; k < n; k++) {
                        if (getBit(mask ^ (1 << j), k))
                            if (firstDP[j][mask] > firstDP[k][mask ^ (1 << j)] + getDist(k, j)) {
                                firstDP[j][mask] = firstDP[k][mask ^ (1 << j)] + getDist(k, j);
                                secondDP[j][mask] = k;
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
                        if (getBit(mask ^ (1 << j), k))
                            if (firstDP[j][mask] > firstDP[k][mask ^ (1 << j)] + getDist(k, j)) {
                                firstDP[j][mask] = firstDP[k][mask ^ (1 << j)] + getDist(k, j);
                                secondDP[j][mask] = k;
                            }
                    }
                }
            }
        }
    }

    long long ans = INF;
    for (int i = 1; i < n; i++)
        if (ans > firstDP[i][(maskSize - 1) ^ 1] + getDist(i, 0)) {
            ans = firstDP[i][(maskSize - 1) ^ 1] + getDist(i, 0);
            secondDP[0][(maskSize - 1)] = i;
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
    return 0;
}