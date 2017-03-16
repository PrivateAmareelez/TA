#include <vector>
#include <stack>
#include <list>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("input.txt");
ofstream cout("output.txt");
#else
#include <iostream>
#endif

vector<list<int> > g;
vector<int> degree;
stack<int> s;
vector<char> flag;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;

    g.resize(n);
    degree.resize(n, 0);
    flag.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> degree[i];

        for (int j = 0; j < degree[i]; j++) {
            int v;
            cin >> v;
            v--;
            g[i].push_back(v);
        }

        if (degree[i] == 1)
            s.push(i);
    }

    int ans = 0;
    while (!s.empty()) {
        int v = s.top();
        s.pop();

        int parent = g[v].front();
        if (flag[parent] == false && flag[v] == false){
            flag[parent] = true;
            ans++;
        }

        degree[parent]--;
        if (degree[parent] == 1)
            s.push(parent);
    }

    cout << ans << "\n";
    return 0;
}