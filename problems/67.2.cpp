#include <vector>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("input.txt");
ofstream cout("output.txt");
#else
#include <iostream>
#endif

int cnt = 1;
vector<vector<char> > g;
vector<int> mark;

void dfs(int v) {
    for (int i = 0; i < g.size(); i++) {
        if (g[v][i] == '1' && mark[i] == -1) {
            mark[i] = cnt++;
            dfs(i);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;

    g.resize(n, vector<char>(n, 0));
    mark.resize(n, -1);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            cin >> g[i][j];
        }


    for (int i = 0; i < n; i++) {
        if (mark[i] == -1) {
            mark[i] = cnt++;
            dfs(i);
        }
    }

    for (int i = 0; i < n; i++) {
        cout << mark[i] << " ";
    }
    cout << "\n";
    return 0;
}