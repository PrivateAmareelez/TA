#include <vector>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("input.txt");
ofstream cout("output.txt");
#else
#include <iostream>
#endif

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;
    vector<int> v(n);
    for (int i = 0; i < n; i++)
        cin >> v[i];

    vector<vector<long long> > force(n, vector<long long>(n, 0));
    for (int i = 0; i < n; i++)
        for (int j = i; j < n; j++)
            for (int k = i; k < j; k++)
                for (int l = k + 1; l <= j; l++)
                    force[i][j] += v[k] * v[l];

    vector<vector<long long> > dp(n, vector<long long>(m + 1, LLONG_MAX));

    for (int i = 0; i < n; i++)
        dp[i][0] = force[0][i];
    for (int j = 0; j <= m; j++)
        dp[0][j] = 0;

    for (int i = 1; i < n; i++) {
        for (int j = 1; j <= m; j++) {
            long long ifBombingNow = LLONG_MAX;
            for (int k = 1; k <= i; k++)
                ifBombingNow = min(ifBombingNow, dp[k - 1][j - 1] + force[k][i]);

            dp[i][j] = min(dp[i][j - 1], ifBombingNow);
        }
    }

    cout << dp[n - 1][m] << "\n";
    return 0;
}