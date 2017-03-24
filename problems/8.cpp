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
    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }

    vector<vector<int> > dp(m + 2, vector<int>(n, 0));

    for (int i = 1; i <= m + 1; i++) {
        for (int j = n - 1; j >= 0; j--) {
            int maxWithRapture = 0;
            for (int k = j + 1; k < n; k++)
                maxWithRapture = max(maxWithRapture, dp[i - 1][k]);

            int maxWithoutRapture = 0;
            for (int k = j + 1; k < n; k++)
                if (v[k] > v[j])
                    maxWithoutRapture = max(maxWithoutRapture, dp[i][k]);

            dp[i][j] = max(maxWithRapture, maxWithoutRapture) + 1;
        }
    }

    int ans = 0;
    for (int j = 0; j < n; j++)
        ans = max(ans, dp[m + 1][j]);

    cout << ans << "\n";
    return 0;
}