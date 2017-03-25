#include <vector>
#include <list>

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

    vector<list<int> > g(n);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        g[a].push_back(b);
        g[b].push_back(a);
    }

    for (int i = 0; i < n; i++) {
        cout << g[i].size() << " ";
        for (list<int>::iterator it = g[i].begin(); it != g[i].end(); it++) {
            cout << *it + 1 << " ";
        }
        cout << "\n";
    }
    return 0;
}