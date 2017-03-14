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

vector<int> coverWithRoot, anyCover;
vector<vector<int> > g;
vector<char> usedAny, usedRoot;

int calcCoverWithRoot(int);

int calcAnyCover(int v) {
    if (usedAny[v] == true)
        return anyCover[v];

    if (g[v].size() == 0) {
        anyCover[v] = 0;
        usedAny[v] = true;
        return anyCover[v];
    }

    int sumOfChilds = 0;
    for (int i = 0; i < g[v].size(); i++) {
        sumOfChilds += calcCoverWithRoot(g[v][i]);
    }
    anyCover[v] = min(calcCoverWithRoot(v), sumOfChilds);
    usedAny[v] = true;
    return anyCover[v];
}

int calcCoverWithRoot(int v) {
    if (usedRoot[v] == true)
        return coverWithRoot[v];

    if (g[v].size() == 0) {
        coverWithRoot[v] = 1;
        usedRoot[v] = true;
        return coverWithRoot[v];
    }

    int sumOfChilds = 1;
    for (int i = 0; i < g[v].size(); i++) {
        sumOfChilds += calcAnyCover(g[v][i]);
    }
    coverWithRoot[v] = sumOfChilds;
    usedRoot[v] = true;
    return coverWithRoot[v];
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    g.resize(n);
    coverWithRoot.resize(n);
    anyCover.resize(n);
    usedAny.resize(n, 0);
    usedRoot.resize(n, 0);

    for (int i = 0; i < n; i++) {
        int deg;
        cin >> deg;
        for (int j = 0; j < deg; j++) {
            int x;
            cin >> x;
            x--;
            if (x > i)
                g[i].push_back(x);
        }
    }

    int ans = calcAnyCover(0);
    cout << ans << "\n";
    return 0;
}