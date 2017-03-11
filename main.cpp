#include <vector>
#include <assert.h>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("in.txt");
ofstream cout("out.txt");
#else
#include <iostream>
#endif

const long long mod = 1E9 + 7;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    long long n;
    cin >> n;

    if (n % 2 == 1) {
        cout << 0 << "\n";
        return 0;
    }

    vector<vector<long long> > dp(n + 1, vector<long long>(8, 0));

    dp[0][0] = 1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= 7; j++) {
            switch (j) {
                case 0: {
                    dp[i][3] = (dp[i][3] + dp[i][j]) % mod;
                    dp[i][6] = (dp[i][6] + dp[i][j]) % mod;
                    dp[i + 1][7] = (dp[i + 1][7] + 3 * dp[i][j]) % mod;
                    break;
                }
                case 1: {
                    dp[i][7] = (dp[i][7] + dp[i][j]) % mod;
                    dp[i + 1][6] = (dp[i + 1][6] + 2 * dp[i][j]) % mod;
                    break;
                }
                case 2: {
                    dp[i + 1][5] = (dp[i + 1][5] + dp[i][j]) % mod;
                    break;
                }
                case 3: {
                    dp[i + 1][4] = (dp[i + 1][4] + dp[i][j]) % mod;
                    break;
                }
                case 4: {
                    dp[i][7] = (dp[i][7] + dp[i][j]) % mod;
                    dp[i + 1][3] = (dp[i + 1][3] + 2 * dp[i][j]) % mod;
                    break;
                }
                case 5: {
                    dp[i + 1][2] = (dp[i + 1][2] + dp[i][j]) % mod;
                    break;
                }
                case 6: {
                    dp[i + 1][1] = (dp[i + 1][1] + dp[i][j]) % mod;
                    break;
                }
                case 7: {
                    dp[i + 1][0] = (dp[i + 1][0] + dp[i][j]) % mod;
                    break;
                }
            }
        }
    }

    cout << dp[n - 1][7] << "\n";
    //(4*6^(n/2)+1)/5
    return 0;
}