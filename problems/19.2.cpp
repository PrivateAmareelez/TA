#include <vector>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("concert.in");
ofstream cout("concert.out");
#else
#include <iostream>
#endif

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    long long d;
    cin >> n >> m >> d;

    vector<long long> v(n);
    for (int i = 0; i < n; i++)
        cin >> v[i];

    vector<pair<int, long long> > dp(n + 1);
    dp[0] = make_pair(0, 0);
    int recorded = 1;

    for (int i = 0; i < n; i++) {
        if (v[i] > d)
            continue;

        if (recorded == 0) {
            dp[recorded] = make_pair(1, v[i]);
            recorded++;
        } else if (dp[recorded - 1].second + v[i] <= d) {
            dp[recorded] = make_pair(dp[recorded - 1].first, dp[recorded - 1].second + v[i]);
            recorded++;
        } else if (dp[recorded - 1].first + 1 < m) {
            dp[recorded] = make_pair(dp[recorded - 1].first + 1, v[i]);
            recorded++;
        }

        for (int j = recorded - 2; j >= 0; j--) {
            pair<int, long long> curr = dp[j];

            if (dp[j].second + v[i] <= d) {
                curr.second += v[i];
                dp[j + 1] = min(dp[j + 1], curr);
            } else if (curr.first + 1 < m) {
                curr = make_pair(curr.first + 1, v[i]);
                dp[j + 1] = min(dp[j + 1], curr);
            }
        }
    }

    cout << recorded - 1 << "\n";
    return 0;
}