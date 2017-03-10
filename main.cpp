#include <vector>
#include <set>

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
    vector<long long> songs(n);
    for (int i = 0; i < n; i++)
        cin >> songs[i];

    vector<vector<long long> > v(n, vector<long long>(n, 0));
    for (int i = n - 1; i >= 0; i--) {
        multiset<long long, greater<long long> > s;
        long long currTime = 0;

        for (int j = i; j < n; j++) {
            if (currTime + songs[j] <= d) {
                s.insert(songs[j]);
                currTime += songs[j];
            } else if (!s.empty() && *s.begin() > songs[j]) {
                currTime -= *s.begin() - songs[j];
                s.erase(s.begin());
                s.insert(songs[j]);
            }
            v[i][j] = s.size();
        }
    }

    vector<vector<long long> > dp(n, vector<long long>(m + 1, 0));
    for (int i = 0; i < n; i++)
        dp[i][1] = v[0][i];

    for (int i = 0; i < n; i++) {
        for (int j = 2; j <= m; j++) {
            long long tempAns = 0;
            for (int k = 0; k < i; k++) {
                tempAns = max(tempAns, dp[k][j - 1] + v[k + 1][i]);
            }

            dp[i][j] = max(v[0][i], tempAns);
        }
    }

    cout << dp[n - 1][m] << "\n";

    return 0;
}