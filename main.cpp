#include <vector>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>
#include <map>

ifstream cin("input.txt");
ofstream cout("output.txt");
#else
#include <iostream>
#endif

vector<vector<long long> > dp;
vector<pair<int, int> > dims;

long long calc(int i, int j) {
    if (dp[i][j] != LLONG_MAX)
        return dp[i][j];

    long long ans = LLONG_MAX;
    for (int k = i; k < j; k++) {
        ans = min(ans, calc(i, k) + calc(k + 1, j) + dims[i].first * dims[k].second * dims[j].second);
    }
    dp[i][j] = ans;
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    dims.resize(n);
    for (int i = 0; i < n; i++)
        cin >> dims[i].first >> dims[i].second;

    dp.resize(n, vector<long long>(n, LLONG_MAX));
    for (int i = 0; i < n; i++)
        dp[i][i] = 0;

    cout << calc(0, n - 1) << "\n";
    return 0;
}